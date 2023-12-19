/* BSD 2-Clause License
 * 
 * Copyright (c) 2023, Felipe Bombardelli
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
	
// ============================================================================
//  Header
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <msgpack.h>

#include "lt_api.h"

typedef struct {
	msgpack_sbuffer sbuf;
	msgpack_packer pk;
} ll_encoder_t;

// ============================================================================
//  msgpack
// ============================================================================

static
int lt_enc_msgpack_put_u32(link_t* link, uint32_t val) {
	ll_encoder_t* enc_obj = link->enc_obj;
	if ( enc_obj ) {
		msgpack_pack_int32(&enc_obj->pk, val);
	}
	return 0;
}

static
int lt_enc_msgpack_put_i32(link_t* link, int32_t val) {
	ll_encoder_t* enc_obj = link->enc_obj;
	if ( enc_obj ) {
		msgpack_pack_int32(&enc_obj->pk, val);
	}
	return 0;
}

static
int lt_enc_msgpack_put_f32(link_t* link, float val) {
	ll_encoder_t* enc_obj = link->enc_obj;
	if ( enc_obj ) {
		msgpack_pack_float(&enc_obj->pk, val);
	}
	return 0;
}

static
int lt_enc_msgpack_put_str(link_t* link, const char* val) {
	ll_encoder_t* enc_obj = link->enc_obj;
	if ( enc_obj ) {
		const size_t size = strlen(val);
		msgpack_pack_str(&enc_obj->pk, size);
		msgpack_pack_str_body(&enc_obj->pk, val, size);
	}
	return 0;
}

static
int lt_enc_msgpack_put_arr(link_t* link, const void* arr_ptr, char type, size_t arr_size) {
	ll_encoder_t* enc_obj = link->enc_obj;
	if ( type == 'i' ) {
		const int* arr_i32_ptr = (int*) arr_ptr;
		msgpack_pack_array(&enc_obj->pk, arr_size);
		for (size_t i=0; i<arr_size; i++) {
			msgpack_pack_int(&enc_obj->pk, arr_i32_ptr[i]);
		}
	} else if ( type == 'f' ) {
		const float* arr_f32_ptr = (float*) arr_ptr;
		msgpack_pack_array(&enc_obj->pk, arr_size);
		for (size_t i=0; i<arr_size; i++) {
			msgpack_pack_float(&enc_obj->pk, arr_f32_ptr[i]);
		}

	}
}

static
int lt_enc_msgpack_put_cmd(link_t* link, char cmd) {
	ll_encoder_t* enc_obj = link->enc_obj;

	if ( cmd == '\n' ) {
		const size_t size = enc_obj->sbuf.size;
		const char* data = enc_obj->sbuf.data;
		lt_write(link, data, size);
		msgpack_sbuffer_clear(&enc_obj->sbuf);
	}

	return 0;
}

static
lt_encoder_api_t lt_enc_msgpack = {
	.put_u32 = lt_enc_msgpack_put_u32,
	.put_i32 = lt_enc_msgpack_put_i32,
	.put_f32 = lt_enc_msgpack_put_f32,
	.put_str = lt_enc_msgpack_put_str,
	.put_arr = lt_enc_msgpack_put_arr,

	.put_cmd = lt_enc_msgpack_put_cmd
};

int lt_new_msgpack_obj(link_t* link, const lt_args_t* args) {
	link->enc_api = &lt_enc_msgpack;

	ll_encoder_t* enc_obj = malloc( sizeof(ll_encoder_t) );
	msgpack_sbuffer_init(&enc_obj->sbuf);
	msgpack_packer_init(&enc_obj->pk, &enc_obj->sbuf, msgpack_sbuffer_write);
	link->enc_obj = enc_obj;

	return 0;
}

