/**
 * Author: simon lindholm
 * Date: 2015-03-19
 * License: CC0
 * Source: me
 * Description:
 */
#pragma once

auto step = [&] { ...; ++i; };
int i = from;
while (i & 3 && i < to) step(); // for alignment, if needed
while (i + 4 <= to) { step(); step(); step(); step(); }
while (i < to) step();
