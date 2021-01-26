#include <hashbrown/sound/modulator.h>
#include <hashbrown/sound/operator.h>

void modulator_process (modulator_t *modulator) {

    /* TODO: generalize oscillators and waveforms 
     * and make sure noise is one of the function generators */

    if (modulator->operator->noise_mode) {
        modulator->output = modulator->modulation_index
            * modulator->operator->noise_generator.output;
    } else {
        modulator->output = modulator->modulation_index
            * modulator->operator->oscillator.output;
    }
}
