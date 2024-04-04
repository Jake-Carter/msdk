/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#define MULT(x, y) ((x) * (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define NUM_ARS     4
#define NUM_SCALES  4
#define NUM_CLASSES 2

#define LOC_DIM 4
#define KPTS_DIM 8
#define NUM_PRIORS_PER_AR 387
#define NUM_PRIORS        NUM_PRIORS_PER_AR* NUM_ARS

#define MAX_PRIORS  20

#define MIN_CLASS_SCORE     50 // ~0.5*256
#define MAX_ALLOWED_OVERLAP 0.1f

#define TFT_W 320
#define TFT_H 240
#define TFT_X_OFFSET 0

void get_priors(void);
void get_prior_locs(void);
void get_prior_kpts(void);
void get_prior_cls(void);
int get_prior_idx(int ar_idx, int scale_idx, int rel_idx);
void get_indices(int* ar_idx, int* scale_idx, int* rel_idx, int prior_idx);
void calc_softmax(int8_t *prior_cls_vals, int prior_idx);
int8_t check_for_validity(int8_t *cl_addr);
void check_for_all_zero_output(void);

void nms_memory_init(void);
void nms(void);
void insert_nms_prior(uint8_t val, int idx, uint8_t *val_arr, int *idx_arr, int *arr_len);
void insert_val(uint8_t val, uint8_t *arr, int arr_len, int idx);
void insert_idx(int val, int *arr, int arr_len, int idx);
float calculate_IOU(float *box1, float *box2);

void get_cxcy(float *cxcy, int prior_idx);
void gcxgcy_to_cxcy(float *cxcy, int prior_idx, float *priors_cxcy);
void cxcy_to_xy(float *xy, float *cxcy);
void get_kpts_coords(float* kpts, float* xy, int prior_idx);

void print_detected_boxes(float *xy, float *kpts);
void draw_obj_rect(float *xy, uint32_t w, uint32_t h, uint8_t scale);