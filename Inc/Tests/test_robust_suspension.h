
#ifndef _TEST_ROBUST_SUSPENSION_H_
#define _TEST_ROBUST_SUSPENSION_H_

#include <assert.h>
#include <math.h>
#include "tests/test_base.h"
#include "robust_suspension.h"

class TestRobustSuspension : public TestBase {
public:
	TestRobustSuspension();

	void init();
	void testInitialize();
	void testActive_1();
	void testActive_2();

};

#endif /* _TEST_ROBUST_SUSPENSION_H_ */
