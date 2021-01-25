#include <hashbrown/sound/modulator.h>
#include <hashbrown/sound/operator.h>

void modulator_process (modulator_t *modulator) {

    modulator->output = modulator->modulation_index
        * modulator->operator->oscillator.output;
}
