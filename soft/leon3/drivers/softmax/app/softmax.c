#include "libesp.h"
#include "cfg.h"

static unsigned in_words_adj;
static unsigned out_words_adj;
static unsigned in_len;
static unsigned out_len;
static unsigned in_size;
static unsigned out_size;
static unsigned out_offset;
static unsigned size_;

float abs_float(const float input)
{
    return input < 0 ? -input : input;
}

float allowed_error = 0.001;

/* User-defined code */
static int validate_buffer(token_t *out, token_t *gold)
{
	int i;
	int j;
	unsigned errors = 0;

    for (i = 0; i < 1; i++) {
		for (j = 0; j < size; j++)
        {
            token_t gold_data_fxd = gold[i * out_words_adj + j];
            token_t out_data_fxd = out[i * out_words_adj + j];
            float gold_data_flt = fixed32_to_float(gold_data_fxd, 2);
            float out_data_flt = fixed32_to_float(out_data_fxd, 2);
            float error_it = abs_float(gold_data_flt - out_data_flt);

			if (error_it > allowed_error)
            {
				errors++;
            }
        }
    }

	return errors;
}

// Returns approximate value of e^x,
// using sum of first n terms of Taylor Series  
static float exponential(int n, float x)  
{
    float sum = 1.0f; // initialize sum of series
    int i;
    for (i = n - 1; i > 0; --i )
        sum = 1 + x * sum / i;  
                    
    return sum;  
}  

static void softmax_sw(float *input, float *output)
{
    float exp_in[size];
    float sum_exp = 0;
    unsigned i;
    for (i = 0; i < size; i++) {
        exp_in[i] = exponential(10, input[i]);
        sum_exp += exp_in[i];
    }
    for (i = 0; i < size; i++) {
        output[i] = exp_in[i] / sum_exp;
    }
}


/* User-defined code */
static void init_buffer(token_t *in, token_t * gold)
{
	int i;
	int j;

	for (i = 0; i < 1; i++)
    {
		for (j = 0; j < size; j++)
        {
            float data_flt = ((i * size + j) % 32) + 0.25;
            token_t data_fxd = 0xdeadbeef00000000 | float_to_fixed32(data_flt, 6);
			in[i * in_words_adj + j] = (token_t) data_fxd;
        }
    }

    float in_local_gold[size];
    float out_local_gold[size];
	for (i = 0; i < 1; i++)
    {
		for (j = 0; j < size; j++)
        {
			in_local_gold[i * size + j] = ((i * size + j) % 32) + 0.25;
        }
    }
    softmax_sw(in_local_gold, out_local_gold);

    for (i = 0; i < 1; i++)
    {
		for (j = 0; j < size; j++)
        {
            float data_flt = out_local_gold[i * size + j];
            token_t data_fxd = float_to_fixed32(data_flt, 2);
			gold[i * out_words_adj + j] = 0xdeadbeef00000000 | (token_t) data_fxd;
        }
    }
}


/* User-defined code */
static void init_parameters()
{
	if (DMA_WORD_PER_BEAT(sizeof(token_t)) == 0) {
		in_words_adj = size;
		out_words_adj = size;
	} else {
		in_words_adj = round_up(size, DMA_WORD_PER_BEAT(sizeof(token_t)));
		out_words_adj = round_up(size, DMA_WORD_PER_BEAT(sizeof(token_t)));
	}
	in_len = in_words_adj * (1);
	out_len =  out_words_adj * (1);
	in_size = in_len * sizeof(token_t);
	out_size = out_len * sizeof(token_t);
	out_offset = in_len;
	size_ = (out_offset * sizeof(token_t)) + out_size;
}


int main(int argc, char **argv)
{
	int errors;

	token_t *gold;
	token_t *buf;

	init_parameters();

	buf = (token_t *) esp_alloc(size);
	gold = malloc(out_size);

	init_buffer(buf, gold);

	printf("\n====== %s ======\n\n", cfg_000[0].devname);
	/* <<--print-params-->> */
	printf("  .size = %d\n", size);
	printf("  .batch = %d\n", batch);
	printf("\n  ** START **\n");

	esp_run(cfg_000, NACC);

	printf("\n  ** DONE **\n");

	errors = validate_buffer(&buf[out_offset], gold);

	free(gold);
	esp_cleanup();

	if (!errors)
		printf("+ Test PASSED\n");
	else
		printf("+ Test FAILED\n");

	printf("\n====== %s ======\n\n", cfg_000[0].devname);

	return errors;
}