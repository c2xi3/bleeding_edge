// Copyright (c) 2014, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

library shelf.message_test;

import 'dart:async';
import 'dart:convert';

import 'package:shelf/src/message.dart';
import 'package:unittest/unittest.dart';

class _TestMessage extends Message {
  _TestMessage(Map<String, String> headers, Stream<List<int>> body)
      : super(body, headers: headers);
}

Message _createMessage({Map<String, String> headers, Stream<List<int>> body}) {
  if (body == null) body = new Stream.fromIterable([]);
  return new _TestMessage(headers, body);
}

void main() {
  group('headers', () {
    test('message headers are case insensitive', () {
      var message = _createMessage(headers: {'foo': 'bar'});

      expect(message.headers, containsPair('foo', 'bar'));
      expect(message.headers, containsPair('Foo', 'bar'));
      expect(message.headers, containsPair('FOO', 'bar'));
    });

    test('null header value becomes empty, immutable', () {
      var message = _createMessage();
      expect(message.headers, isEmpty);
      expect(() => message.headers['h1'] = 'value1', throwsUnsupportedError);
    });

    test('headers are immutable', () {
      var message = _createMessage(headers: {'h1': 'value1'});
      expect(() => message.headers['h1'] = 'value1', throwsUnsupportedError);
      expect(() => message.headers['h1'] = 'value2', throwsUnsupportedError);
      expect(() => message.headers['h2'] = 'value2', throwsUnsupportedError);
    });
  });
  group("readAsString", () {
    test("supports a null body", () {
      var request = _createMessage();
      expect(request.readAsString(), completion(equals("")));
    });

    test("supports a Stream<List<int>> body", () {
      var controller = new StreamController();
      var request = _createMessage(body: controller.stream);
      expect(request.readAsString(), completion(equals("hello, world")));

      controller.add([104, 101, 108, 108, 111, 44]);
      return new Future(() {
        controller
          ..add([32, 119, 111, 114, 108, 100])
          ..close();
      });
    });

    test("defaults to UTF-8", () {
      var request = _createMessage(body: new Stream.fromIterable([[195, 168]]));
      expect(request.readAsString(), completion(equals("è")));
    });

    test("the content-type header overrides the default", () {
      var request = _createMessage(
          headers: {'content-type': 'text/plain; charset=iso-8859-1'},
          body: new Stream.fromIterable([[195, 168]]));
      expect(request.readAsString(), completion(equals("Ã¨")));
    });

    test("an explicit encoding overrides the content-type header", () {
      var request = _createMessage(
          headers: {'content-type': 'text/plain; charset=iso-8859-1'},
          body: new Stream.fromIterable([[195, 168]]));
      expect(request.readAsString(LATIN1), completion(equals("Ã¨")));
    });
  });

  group("read", () {
    test("supports a null body", () {
      var request = _createMessage();
      expect(request.read().toList(), completion(isEmpty));
    });

    test("supports a Stream<List<int>> body", () {
      var controller = new StreamController();
      var request = _createMessage(body: controller.stream);
      expect(request.read().toList(), completion(equals([
        [104, 101, 108, 108, 111, 44],
        [32, 119, 111, 114, 108, 100]
      ])));

      controller.add([104, 101, 108, 108, 111, 44]);
      return new Future(() {
        controller
          ..add([32, 119, 111, 114, 108, 100])
          ..close();
      });
    });
  });

  group("contentLength", () {
    test("is null without a content-length header", () {
      var request = _createMessage();
      expect(request.contentLength, isNull);
    });

    test("comes from the content-length header", () {
      var request = _createMessage(headers: {
        'content-length': '42'
      });
      expect(request.contentLength, 42);
    });
  });

  group("mimeType", () {
    test("is null without a content-type header", () {
      expect(_createMessage().mimeType, isNull);
    });

    test("comes from the content-type header", () {
      expect(_createMessage(headers: {
        'content-type': 'text/plain'
      }).mimeType, equals('text/plain'));
    });

    test("doesn't include parameters", () {
      expect(_createMessage(headers: {
        'content-type': 'text/plain; foo=bar; bar=baz'
      }).mimeType, equals('text/plain'));
    });
  });

  group("encoding", () {
    test("is null without a content-type header", () {
      expect(_createMessage().encoding, isNull);
    });

    test("is null without a charset parameter", () {
      expect(_createMessage(headers: {
        'content-type': 'text/plain'
      }).encoding, isNull);
    });

    test("is null with an unrecognized charset parameter", () {
      expect(_createMessage(headers: {
        'content-type': 'text/plain; charset=fblthp'
      }).encoding, isNull);
    });

    test("comes from the content-type charset parameter", () {
      expect(_createMessage(headers: {
        'content-type': 'text/plain; charset=iso-8859-1'
      }).encoding, equals(LATIN1));
    });
  });
}
