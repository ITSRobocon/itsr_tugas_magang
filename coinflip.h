#pragma once

// Plays the ASCII coin flip animation and lands on the given side.
// resultHeads = true  -> lands on HEADS
// resultHeads = false -> lands on TAILS
// The result is decided by the caller; this only animates it.
void playCoinFlipAnimation(bool resultHeads);