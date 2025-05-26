#ifndef __FILEUUID10d55c41e2364e83b78d3f1c85e8c03b
#define __FILEUUID10d55c41e2364e83b78d3f1c85e8c03b

#define FSC_INC_OBJECTS_VSHELL_PUBLIC
#ifdef FSC_INC_OBJECTS_VSHELL_PRIVATE
#define FSC_INC_OBJECTS_VSHELL_PROTECTED
#endif
typedef struct FSCTL_VSHELL_TYPE
{
#ifdef FSC_INC_OBJECTS_VSHELL_PUBLIC
    void (*const entry)(void);
#endif
#ifdef FSC_INC_OBJECTS_VSHELL_PROTECTED
    const volatile char *const pwd;
#endif
#ifdef FSC_INC_OBJECTS_VSHELL_PRIVATE
    int retval;
#endif
} fsc_vshell_t;

extern fsc_vshell_t fscVShell;

#endif /** __FILEUUID10d55c41e2364e83b78d3f1c85e8c03b */
