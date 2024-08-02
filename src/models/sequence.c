#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "models.h"
#include "../utils/utils.h"
#include "../types/types.h"

extern const char *INSUFICIENT_MEMORY, *NO_ERROR;

sequence_t *new_sequence(double begin, double end, float step, bool isLoop) {
	if(step == 0)
		PrintError("El valor de cambio debe ser diferente de %d.\n", (double)0.0);

	sequence_t *NewSequence = (sequence_t*) malloc(sizeof(sequence_t));
	if(NewSequence == NULL)
		PrintError(INSUFICIENT_MEMORY, "struct sequence_t");

	NewSequence->o = SEQUENCE;
	NewSequence->begin = NewSequence->current = begin;
	NewSequence->end = end;
	NewSequence->step = step;
	NewSequence->isLoop = isLoop;
	return NewSequence;
}

sequence_t *clone_sequence(sequence_t sequence) {
	return new_sequence(sequence.begin, sequence.end, sequence.step, sequence.isLoop);
}

result_t *use_sequence(sequence_t *sequence) {
	result_t *result = new_result();
	if(!sequence->isLoop && sequence->isEnd)
		return result_set_error(result, "La secuencia ha llegado a su final.");
	Double *current = new_double(sequence->current);

	float step = sequence->step;
	if(sequence->isEnd)
		sequence->current = sequence->begin - step;
	sequence->current += sequence->step;
	sequence->isEnd = false;

	if((step < 0 && sequence->current <= sequence->end)
	   || (step > 0 && sequence->current >= sequence->end))
		sequence->isEnd = true;

	return result_set_value(result, current);
}

const sequence_t DEFAULT_SEQUENCE = {
	.o = SEQUENCE,
	.begin = 1,
	.current = 1,
	.end = 1999999,
	.step = 1,
	.isLoop = false
};
