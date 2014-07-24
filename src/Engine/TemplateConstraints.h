#pragma once

template<typename derived, typename base>
struct Derives
{
	static void constraints(derived* p){base * pb = p;}
	Derives(){ void(*p)(derived*) = constraints;}
};