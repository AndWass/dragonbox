// Copyright 2020 Junekey Jeon
//
// The contents of this file may be used under the terms of
// the Apache License v2.0 with LLVM Exceptions.
//
//    (See accompanying file LICENSE-Apache or copy at
//     https://llvm.org/foundation/relicensing/LICENSE.txt)
//
// Alternatively, the contents of this file may be used under the terms of
// the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE-Boost or copy at
//     https://www.boost.org/LICENSE_1_0.txt)
//
// Unless required by applicable law or agreed to in writing, this software
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.

#include "../dragonbox_to_chars.h"
#include "../benchmark/ryu/ryu.h"

#include <iostream>
#include <iomanip>
#include <string_view>

template <class Float>
static void test_all_shorter_interval_cases_impl()
{
	using ieee754_traits = jkj::dragonbox::ieee754_traits<Float>;
	using ieee754_format_info = jkj::dragonbox::ieee754_format_info<ieee754_traits::format>;
	using carrier_uint = typename ieee754_traits::carrier_uint;

	char buffer1[64];
	char buffer2[64];

	bool succeeded = true;
	for (int e = ieee754_format_info::min_exponent + 1;
		e <= ieee754_format_info::max_exponent; ++e)
	{
		// Compose a floating-point number
		carrier_uint br = carrier_uint(e) << ieee754_format_info::significand_bits;
		auto x = jkj::dragonbox::ieee754_bits<Float>{ br }.to_float();

		jkj::dragonbox::to_chars(x, buffer1);
		if constexpr (std::is_same_v<Float, float>) {
			f2s_buffered(x, buffer2);
		}
		else {
			d2s_buffered(x, buffer2);
		}

		std::string_view view1(buffer1);
		std::string_view view2(buffer2);

		if (view1 != view2) {
			std::cout << "Error detected! [Ryu = " << buffer2
				<< ", Dragonbox = " << buffer1 << "]\n";
			succeeded = false;
		}
	}

	if (succeeded) {
		std::cout << "All cases are verified.\n";
	}
	else {
		std::cout << "Error detected.\n";
	}
}

void test_all_shorter_interval_cases()
{
	std::cout << "[Testing all shorter interval cases for binary32...]\n";
	test_all_shorter_interval_cases_impl<float>();
	std::cout << "Done.\n\n\n";

	std::cout << "[Testing all shorter interval cases for binary64...]\n";
	test_all_shorter_interval_cases_impl<double>();
	std::cout << "Done.\n\n\n";
}