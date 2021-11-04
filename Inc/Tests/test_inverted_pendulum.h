
#ifndef _TEST_INVERTED_PENDULUM_H_
#define _TEST_INVERTED_PENDULUM_H_

#include <assert.h>
#include "Tests/test_base.h"
#include "inverted_pendulum.h"

#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif

class TestInvertedPendulum : public TestBase {
public:
	TestInvertedPendulum();

	void init();
	void testInitialize();
	void testU();
	void testPendcart();
	void testRungeKutta4();

};

#endif /* _TEST_INVERTED_PENDULUM_H_ */
