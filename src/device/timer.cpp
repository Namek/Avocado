#include "timer.h"
#include "system.h"

using namespace timer;

template <int which>
Timer<which>::Timer(System* sys) : sys(sys) {}

template <int which>
void Timer<which>::step(int cycles) {
    cnt += cycles;

    if (which == 0) {
        if (mode.clockSource0 == CounterMode::ClockSource0::dotClock) {
            current._reg += cnt;
            cnt = 0;
        } else {  // System Clock
            current._reg += cnt / 3;
            cnt %= 3;
        }
    } else if (which == 1) {
        if (mode.clockSource1 == CounterMode::ClockSource1::hblank) {
            current._reg += cnt / 3413;
            cnt %= 3413;
        } else {  // System Clock
            current._reg += cnt / 3;
            cnt %= 3;
        }
    } else if (which == 2) {
        if (mode.clockSource2 == CounterMode::ClockSource2::systemClock_8) {
            current._reg += cnt / (8 * 3);
            cnt %= 8 * 3;
        } else {  // System Clock
            current._reg += cnt / 3;
            cnt %= 3;
        }
    }

    if (current._reg >= target._reg) {
        mode.reachedTarget = true;
        if (mode.resetToZero == CounterMode::ResetToZero::whenTarget && target._reg != 0) current._reg = 0;  // TODO: HAX?
    }

    if (current._reg >= 0xffff) {
        mode.reachedFFFF = true;
        if (mode.resetToZero == CounterMode::ResetToZero::whenFFFF) current._reg = 0;
    }

    if (mode.irqWhenTarget || mode.irqWhenFFFF) {
        if (mode.irqRepeatMode == CounterMode::IrqRepeatMode::repeatedly
            || (mode.irqRepeatMode == CounterMode::IrqRepeatMode::oneShot && !irqOccured)) {
            if (mode.irqPulseMode == CounterMode::IrqPulseMode::toggle)
                mode.interruptRequest = !mode.interruptRequest;
            else
                mode.interruptRequest = false;
        }
    }

    if (mode.interruptRequest == false && !irqOccured) {
        sys->interrupt->trigger(mapIrqNumber());
        irqOccured = true;
    }
}

template <int which>
uint8_t Timer<which>::read(uint32_t address) {
    if (address < 2) {
        return current.read(address);
    }
    if (address >= 4 && address < 8) {
        uint8_t v = mode.read(address - 4);
        if (address == 7) {
            mode.reachedFFFF = false;
            mode.reachedTarget = false;
        }
        return v;
    }
    if (address >= 8 && address < 12) {
        return target.read(address - 8);
    }
    return 0;
}

template <int which>
void Timer<which>::write(uint32_t address, uint8_t data) {
    if (address < 2) {
        current.write(address, data);
    } else if (address >= 4 && address < 8) {
        current._reg = 0;
        irqOccured = false;
        mode.write(address - 4, data);  // BIOS uses 0x0148 for TIMER1
    } else if (address >= 8 && address < 12) {
        target.write(address - 8, data);
    }
}

template class Timer<0>;
template class Timer<1>;
template class Timer<2>;