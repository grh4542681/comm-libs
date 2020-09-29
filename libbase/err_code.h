#ifndef __ERR_CODE_H__
#define __ERR_CODE_H__

//0 ~ 0x00001000 is system errno
#define ERROR_CODE_BASE (0x00001000)
#define ERROR_CODE_SPACE (0x00000100)

#define PUBLIC_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 1))
#define TIMER_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 2))
#define IO_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 3))
#define FILE_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 4))
#define PROCESS_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 5))
#define MODEL_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 6))
#define SIGNAL_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 7))
#define THREAD_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 8))
#define CONTAINER_ERROR_CODE_MODULE  (ERROR_CODE_BASE + (ERROR_CODE_SPACE * 9))

#endif
