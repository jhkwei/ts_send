#include <stdio.h>
#include <stdlib.h>

struct ring_fifo{
	int head;
	int tail;
	int size;
	int length;
	void **data;
};

struct ts_node{
	int  delta_time;
	int size;
	int head;
	void *data;
};

#define ts_malloc malloc
#define ts_free free
#define ts_warn(fmt, args...) do{printf("[warn] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_error(fmt, args...) do{printf("[Error] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_info(fmt, args...) do{printf("[Info] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_trace(fmt, args...) do{printf("[Trace] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__);printf(fmt,##args);}while(0)
#define ts_debug(fmt, args...) do{printf("[Debug] Line:%d Func:%s File:%s\n", __LINE__, __FUNCTION__, __FILE__)}while(0)

struct ring_fifo *ring_fifo_resize(struct ring_fifo *r){
	assert(r);
	int length = r->length>0?r->length * 2:2;
	void *data = ts_malloc(length);
	if(!data){
		ts_warn("malloc failed\n");
		return NULL;
	}
	int i;
	int j = 0;
	for(i = 0; i < r->size; i++){
		j = (r->head + i)%r->length;
		data[i] = r->data[j];
	}
	r->tail = j;
	r->length = length;
	return r;
}

struct ring_fifo_new(){
	struct ring_fifo_new *r = ts_malloc(sizeof(*r));
	if(!r){
		ts_warn("malloc failed\n");
		return NULL;
	}
	memset(r, 0, sizeof(*r));
	return ring_fifo_resize(r);
}

void ring_fifo_free(struct ring_fifo *r){
	assert(r);
	ts_free(r);
}

bool ring_fifo_push(struct ring_fifo *r, void *node){
	assert(r);
	if(r->size == r->length){
		if(!ring_fifo_resize(r)){
			ts_warn("ring fifo resize failed, length:%d\n", r->length);
			return false;
		}
	}
	r->tail = r->tail%r->length;
	r->data[r->tail] = node;
	r->tail++;
	r->size++;
	return true;
}

void *ring_fifo_pop(struct ring_fifo *r){
	assert(r);
	if(r->size == 0){
		return NULL;
	}
	r->head++;
	r->size--;
	r->head = r->head%r->length;
	void *data = r->data[r->head];
	return data;
}


