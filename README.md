# pBuffer-agenda


## Tamanhos e offset dos espaços de memória
```c
#define SIZE_PBUFFER            4096
#define SIZE_USER_OPTION       	1
#define SIZE_MESSAGE          	50
#define SIZE_NODE_NAME        	50
#define SIZE_NODE_AGE       	10
#define SIZE_NODE_EMAIL         60
#define SIZE_NODE               (SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL + (2 * sizeof(void*)))
#define SIZE_TMP_CURRENT        (sizeof(void*))
#define SIZE_TMP_FOUND          (sizeof(int))
#define OFFSET_USEROPTION       0
#define OFFSET_MESSAGE          (OFFSET_USEROPTION + SIZE_USER_OPTION)
#define OFFSET_TMP_NODE_NAME    (OFFSET_MESSAGE + SIZE_MESSAGE)
#define OFFSET_TMP_NODE_AGE     (OFFSET_TMP_NODE_NAME + SIZE_NODE_NAME)
#define OFFSET_TMP_NODE_EMAIL   (OFFSET_TMP_NODE_AGE + SIZE_NODE_AGE)
#define OFFSET_TMP_CURRENT      (OFFSET_TMP_NODE_EMAIL + SIZE_NODE_EMAIL)
#define OFFSET_TMP_FOUND        (OFFSET_TMP_CURRENT + SIZE_TMP_CURRENT)
#define SIZE_TMP_AREA           (OFFSET_TMP_FOUND + SIZE_TMP_FOUND)
#define OFFSET_P_HEAD           (SIZE_TMP_AREA)
#define OFFSET_P_NODE_FREE      (OFFSET_P_HEAD + sizeof(void*))
```

## Ponteiros para os espaços de memória do programa utilizando aritmética de ponteiros

```c
#define P_USEROPTION        ((char*)pBuffer + OFFSET_USEROPTION)
#define P_MESSAGE           ((char*)pBuffer + OFFSET_MESSAGE)
#define P_TMP_NODE_NAME     ((char*)pBuffer + OFFSET_TMP_NODE_NAME)       
#define P_TMP_NODE_AGE      ((char*)pBuffer + OFFSET_TMP_NODE_AGE)      
#define P_TMP_NODE_EMAIL    ((char*)pBuffer + OFFSET_TMP_NODE_EMAIL)
#define P_TMP_CURRENT       ((void**)(pBuffer + OFFSET_TMP_CURRENT))
#define P_TMP_FOUND         ((int*)(pBuffer + OFFSET_TMP_FOUND))
#define P_HEAD              ((void**)(pBuffer + OFFSET_P_HEAD))
#define P_NODE_FREE         ((void**)(pBuffer + OFFSET_P_NODE_FREE))
#define P_NODE_AREA         ((char*)pBuffer + OFFSET_P_NODE_FREE + sizeof(void*))
#define NODE_NAME(p)   	    ((char*)(p))
#define NODE_AGE(p)    	    ((char*)(p + SIZE_NODE_NAME))
#define NODE_EMAIL(p)  	    ((char*)(p + SIZE_NODE_NAME + SIZE_NODE_AGE))
#define NODE_NEXT(p)   	    (*((void**)(p + SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL)))
#define NODE_PREV(p)   	    (*((void**)(p + SIZE_NODE_NAME + SIZE_NODE_AGE + SIZE_NODE_EMAIL + sizeof(void*))))
```

