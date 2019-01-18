//
//  test_utils.h
//  smtp_client
//
//  Created by mam on 18.01.2019.
//  Copyright © 2019 mam. All rights reserved.
//

#ifndef test_utils_h
#define test_utils_h

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

#define TEST_FUNCT(name) \
static void test_##name()

#define ADD_SUITE_TEST(suite, name) \
if ((NULL == CU_add_test(suite, #name, (CU_TestFunc)test_##name))) {\
CU_cleanup_registry();\
}

CU_pSuite CUnitCreateSuite(const char* title);
void CUnitInitialize(void);
void CUnitUInitialize(void);

#endif /* test_utils_h */
