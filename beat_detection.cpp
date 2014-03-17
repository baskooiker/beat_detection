#include "../beathistogram/BeatHistogram.h"
#include "..\utils\m_pd.h"
#include <stdlib.h>
#include <Math.h>
#include <time.h>
#include <windows.h>

static t_class *beat_detection_class;

typedef struct _beat_detection {
    t_object x_obj;
    t_outlet *interval_out;
    BeatHistogram* bh;
} t_beat_detection;

void toFloatArray(int argc, t_atom* argv, float array[]) {
    int i;
    for (i = 0; i < argc; i++) {
        array[i] = atom_getfloat(argv + i);
    }
}

void fromFloatArray(int argc, float* array, t_atom* argv) {
    int i;
    for (i = 0; i < argc; i++) {
        SETFLOAT(argv + i, *(array + i));
    }
}

void beat_detection_bang(t_beat_detection *x){
    DWORD now = GetTickCount();
    x->bh->push((float)now);
    float intr = (float)x->bh->interval();
    outlet_float(x->interval_out, (float)intr);
}

void beat_detection_clear(t_beat_detection *x){
    x->bh->clear();
}

void *beat_detection_new(t_symbol *s, int argc, t_atom *argv) {
    t_beat_detection *x = (t_beat_detection *) pd_new(
            beat_detection_class);
    
    x->interval_out = outlet_new(&x->x_obj, &s_float);

    x->bh = new BeatHistogram();

    return (void *) x;
}

extern "C"
void beat_detection_setup(void) {

    beat_detection_class = class_new(gensym("beat_detection"),
            (t_newmethod) beat_detection_new, 0,
            sizeof (t_beat_detection), CLASS_DEFAULT, A_GIMME, 0);

    class_addbang(beat_detection_class, beat_detection_bang);
    
    class_addmethod(beat_detection_class, (t_method)beat_detection_clear, gensym("clear"), A_GIMME, 0);
    
    class_sethelpsymbol(beat_detection_class,
            gensym("beat_detection-help"));
}
