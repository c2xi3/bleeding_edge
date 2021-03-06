// Copyright (c) 2013, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

#include "vm/globals.h"
#if defined(TARGET_ARCH_ARM64)

#include "vm/assembler.h"
#include "vm/cpu.h"
#include "vm/os.h"
#include "vm/unit_test.h"
#include "vm/virtual_memory.h"

namespace dart {

#define __ assembler->

ASSEMBLER_TEST_GENERATE(Simple, assembler) {
  __ add(R0, ZR, Operand(ZR));
  __ add(R0, R0, Operand(42));
  __ ret();
}


ASSEMBLER_TEST_RUN(Simple, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// Move wide immediate tests.
// movz
ASSEMBLER_TEST_GENERATE(Movz0, assembler) {
  __ movz(R0, 42, 0);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movz0, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movz1, assembler) {
  __ movz(R0, 42, 0);  // Overwritten by next instruction.
  __ movz(R0, 42, 1);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movz1, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42LL << 16, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movz2, assembler) {
  __ movz(R0, 42, 2);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movz2, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42LL << 32, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movz3, assembler) {
  __ movz(R0, 42, 3);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movz3, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42LL << 48, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// movn
ASSEMBLER_TEST_GENERATE(Movn0, assembler) {
  __ movn(R0, 42, 0);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movn0, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(~42LL, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movn1, assembler) {
  __ movn(R0, 42, 1);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movn1, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(~(42LL << 16), EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movn2, assembler) {
  __ movn(R0, 42, 2);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movn2, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(~(42LL << 32), EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movn3, assembler) {
  __ movn(R0, 42, 3);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movn3, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(~(42LL << 48), EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}

// movk
ASSEMBLER_TEST_GENERATE(Movk0, assembler) {
  __ movz(R0, 1, 3);
  __ movk(R0, 42, 0);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movk0, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(
      42LL | (1LL << 48), EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movk1, assembler) {
  __ movz(R0, 1, 0);
  __ movk(R0, 42, 1);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movk1, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(
      (42LL << 16) | 1, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movk2, assembler) {
  __ movz(R0, 1, 0);
  __ movk(R0, 42, 2);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movk2, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(
      (42LL << 32) | 1, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(Movk3, assembler) {
  __ movz(R0, 1, 0);
  __ movk(R0, 42, 3);
  __ ret();
}


ASSEMBLER_TEST_RUN(Movk3, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(
      (42LL << 48) | 1, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(MovzBig, assembler) {
  __ movz(R0, 0x8000, 0);
  __ ret();
}


ASSEMBLER_TEST_RUN(MovzBig, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(0x8000, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// add tests.
ASSEMBLER_TEST_GENERATE(AddReg, assembler) {
  __ movz(R0, 20, 0);
  __ movz(R1, 22, 0);
  __ add(R0, R0, Operand(R1));
  __ ret();
}


ASSEMBLER_TEST_RUN(AddReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AddLSLReg, assembler) {
  __ movz(R0, 20, 0);
  __ movz(R1, 11, 0);
  __ add(R0, R0, Operand(R1, LSL, 1));
  __ ret();
}


ASSEMBLER_TEST_RUN(AddLSLReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AddLSRReg, assembler) {
  __ movz(R0, 20, 0);
  __ movz(R1, 44, 0);
  __ add(R0, R0, Operand(R1, LSR, 1));
  __ ret();
}


ASSEMBLER_TEST_RUN(AddLSRReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AddASRReg, assembler) {
  __ movz(R0, 20, 0);
  __ movz(R1, 44, 0);
  __ add(R0, R0, Operand(R1, ASR, 1));
  __ ret();
}


ASSEMBLER_TEST_RUN(AddASRReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AddASRNegReg, assembler) {
  __ movz(R0, 43, 0);
  __ movn(R1, 0, 0);  // R1 <- -1
  __ add(R1, ZR, Operand(R1, LSL, 3));  // R1 <- -8
  __ add(R0, R0, Operand(R1, ASR, 3));  // R0 <- 43 + (-8 >> 3)
  __ ret();
}


ASSEMBLER_TEST_RUN(AddASRNegReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// TODO(zra): test other sign extension modes.
ASSEMBLER_TEST_GENERATE(AddExtReg, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 0xffff, 0);
  __ movk(R1, 0xffff, 1);  // R1 <- -1 (32-bit)
  __ add(R0, R0, Operand(R1, SXTW, 0));  // R0 <- R0 + (sign extended R1)
  __ ret();
}


ASSEMBLER_TEST_RUN(AddExtReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// Loads and Stores.
ASSEMBLER_TEST_GENERATE(SimpleLoadStore, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  __ str(R1, Address(SP, -1*kWordSize, Address::PreIndex));
  __ ldr(R0, Address(SP, 1*kWordSize, Address::PostIndex));
  __ ret();
}


ASSEMBLER_TEST_RUN(SimpleLoadStore, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(SimpleLoadStoreHeapTag, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  __ add(R2, SP, Operand(1));
  __ str(R1, Address(R2, -1, Address::PreIndex));
  __ ldr(R0, Address(R2, -1, Address::PostIndex));
  __ ret();
}


ASSEMBLER_TEST_RUN(SimpleLoadStoreHeapTag, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(LoadStoreLargeIndex, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  // Largest negative offset that can fit in the signed 9-bit immediate field.
  __ str(R1, Address(SP, -32*kWordSize, Address::PreIndex));
  // Largest positive kWordSize aligned offset that we can fit.
  __ ldr(R0, Address(SP, 31*kWordSize, Address::PostIndex));
  // Correction.
  __ add(SP, SP, Operand(kWordSize));  // Restore SP.
  __ ret();
}


ASSEMBLER_TEST_RUN(LoadStoreLargeIndex, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(LoadStoreLargeOffset, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  __ sub(SP, SP, Operand(512*kWordSize));
  __ str(R1, Address(SP, 512*kWordSize, Address::Offset));
  __ add(SP, SP, Operand(512*kWordSize));
  __ ldr(R0, Address(SP));
  __ ret();
}


ASSEMBLER_TEST_RUN(LoadStoreLargeOffset, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(LoadStoreExtReg, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  __ movz(R2, 0xfff8, 0);
  __ movk(R2, 0xffff, 1);  // R2 <- -8 (int32_t).
  // This should sign extend R2, and add to SP to get address,
  // i.e. SP - kWordSize.
  __ str(R1, Address(SP, R2, SXTW));
  __ sub(SP, SP, Operand(kWordSize));
  __ ldr(R0, Address(SP));
  __ add(SP, SP, Operand(kWordSize));
  __ ret();
}


ASSEMBLER_TEST_RUN(LoadStoreExtReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(LoadStoreScaledReg, assembler) {
  __ movz(R0, 43, 0);
  __ movz(R1, 42, 0);
  __ movz(R2, 10, 0);
  __ sub(SP, SP, Operand(10*kWordSize));
  // Store R1 into SP + R2 * kWordSize.
  __ str(R1, Address(SP, R2, UXTX, true));
  __ ldr(R0, Address(SP, R2, UXTX, true));
  __ add(SP, SP, Operand(10*kWordSize));
  __ ret();
}


ASSEMBLER_TEST_RUN(LoadStoreScaledReg, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


// Logical register operations.
ASSEMBLER_TEST_GENERATE(AndRegs, assembler) {
  __ movz(R1, 43, 0);
  __ movz(R2, 42, 0);
  __ and_(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(AndRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AndShiftRegs, assembler) {
  __ movz(R1, 42, 0);
  __ movz(R2, 21, 0);
  __ and_(R0, R1, Operand(R2, LSL, 1));
  __ ret();
}


ASSEMBLER_TEST_RUN(AndShiftRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(BicRegs, assembler) {
  __ movz(R1, 42, 0);
  __ movz(R2, 5, 0);
  __ bic(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(BicRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(OrrRegs, assembler) {
  __ movz(R1, 32, 0);
  __ movz(R2, 10, 0);
  __ orr(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(OrrRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(OrnRegs, assembler) {
  __ movz(R1, 32, 0);
  __ movn(R2, 0, 0);  // R2 <- 0xffffffffffffffff.
  __ movk(R2, 0xffd5, 0);  // R2 <- 0xffffffffffffffe5.
  __ orn(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(OrnRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(EorRegs, assembler) {
  __ movz(R1, 0xffd5, 0);
  __ movz(R2, 0xffff, 0);
  __ eor(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(EorRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(EonRegs, assembler) {
  __ movz(R1, 0xffd5, 0);
  __ movn(R2, 0xffff, 0);
  __ eon(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(EonRegs, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}

// TODO(zra): ands and bics after branches are implemented.


// Logical immediate operations.
ASSEMBLER_TEST_GENERATE(AndImm, assembler) {
  __ movz(R1, 42, 0);
  __ andi(R0, R1, 0xaaaaaaaaaaaaaaaaULL);
  __ ret();
}


ASSEMBLER_TEST_RUN(AndImm, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(AndOneImm, assembler) {
  __ movz(R1, 43, 0);
  __ andi(R0, R1, 1);
  __ ret();
}


ASSEMBLER_TEST_RUN(AndOneImm, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(1, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(OrrImm, assembler) {
  __ movz(R1, 0, 0);
  __ movz(R2, 0x3f, 0);
  __ movz(R3, 0xa, 0);
  __ orri(R1, R1, 0x0020002000200020ULL);
  __ orr(R1, R1, Operand(R3));
  __ and_(R0, R1, Operand(R2));
  __ ret();
}


ASSEMBLER_TEST_RUN(OrrImm, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}


ASSEMBLER_TEST_GENERATE(EorImm, assembler) {
  __ movn(R0, 0, 0);
  __ movk(R0, 0xffd5, 0);  // R0 < 0xffffffffffffffd5.
  __ movz(R1, 0x3f, 0);
  __ eori(R0, R0, 0x3f3f3f3f3f3f3f3fULL);
  __ and_(R0, R0, Operand(R1));
  __ ret();
}


ASSEMBLER_TEST_RUN(EorImm, test) {
  typedef int (*SimpleCode)();
  EXPECT_EQ(42, EXECUTE_TEST_CODE_INT64(SimpleCode, test->entry()));
}

// TODO(zra): andis after branches are implemented.

}  // namespace dart

#endif  // defined(TARGET_ARCH_ARM64)
