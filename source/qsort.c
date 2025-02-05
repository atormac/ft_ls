#include <stdlib.h>
#include <string.h>

static void qsort_swap(void* v1, void* v2, int size) 
{ 
    char buffer[size]; 
  
    memcpy(buffer, v1, size); 
    memcpy(v1, v2, size); 
    memcpy(v2, buffer, size); 
} 
  
static void qsort_sort(void* v, int size, int left, int right, int (*cmp)(const void*, const void*)) 
{ 
    void *vt, *v3; 
    int i, last, mid = (left + right) / 2; 
    if (left >= right) 
        return; 
  
    void* vl = (char*)(v + (left * size)); 
    void* vr = (char*)(v + (mid * size)); 

    qsort_swap(vl, vr, size); 
    last = left; 

    for (i = left + 1; i <= right; i++) { 
  
        vt = (char*)(v + (i * size)); 
        if ((*cmp)(vl, vt) > 0) { 
            ++last; 
            v3 = (char*)(v + (last * size)); 
            qsort_swap(vt, v3, size); 
        } 
    } 
    v3 = (char*)(v + (last * size)); 
    qsort_swap(vl, v3, size); 
    qsort_sort(v, size, left, last - 1, cmp); 
    qsort_sort(v, size, last + 1, right, cmp); 
} 

void qsort_generic(void *v, int nmeb, int size, int (*cmp)(const void *, const void *))
{
	if (size <= 0 || nmeb <= 1)
		return;
	qsort_sort(v, size, 0, nmeb - 1, cmp);
}
