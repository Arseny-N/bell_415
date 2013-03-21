#define MAX_ERR 256

char *ename[MAX_ERR];

static inline void __ename_init(void) 
{
        memset(ename, 0, sizeof(ename));
        ename[0] = "NO_SUCH_ERR";
        ename[EPERM] = "EPERM"; /* 1 */
        ename[ENOENT] = "ENOENT"; /* 2 */
        ename[ESRCH] = "ESRCH"; /* 3 */
        ename[EINTR] = "EINTR"; /* 4 */
        ename[EIO] = "EIO"; /* 5 */
        ename[ENXIO] = "ENXIO"; /* 6 */
        ename[E2BIG] = "E2BIG"; /* 7 */
        ename[ENOEXEC] = "ENOEXEC"; /* 8 */
        ename[EBADF] = "EBADF"; /* 9 */
        ename[ECHILD] = "ECHILD"; /* 10 */
        ename[EAGAIN] = "EAGAIN"; /* 11 */
        ename[ENOMEM] = "ENOMEM"; /* 12 */
        ename[EACCES] = "EACCES"; /* 13 */
        ename[EFAULT] = "EFAULT"; /* 14 */
        ename[ENOTBLK] = "ENOTBLK"; /* 15 */
        ename[EBUSY] = "EBUSY"; /* 16 */
        ename[EEXIST] = "EEXIST"; /* 17 */
        ename[EXDEV] = "EXDEV"; /* 18 */
        ename[ENODEV] = "ENODEV"; /* 19 */
        ename[ENOTDIR] = "ENOTDIR"; /* 20 */
        ename[EISDIR] = "EISDIR"; /* 21 */
        ename[EINVAL] = "EINVAL"; /* 22 */
        ename[ENFILE] = "ENFILE"; /* 23 */
        ename[EMFILE] = "EMFILE"; /* 24 */
        ename[ENOTTY] = "ENOTTY"; /* 25 */
        ename[ETXTBSY] = "ETXTBSY"; /* 26 */
        ename[EFBIG] = "EFBIG"; /* 27 */
        ename[ENOSPC] = "ENOSPC"; /* 28 */
        ename[ESPIPE] = "ESPIPE"; /* 29 */
        ename[EROFS] = "EROFS"; /* 30 */
        ename[EMLINK] = "EMLINK"; /* 31 */
        ename[EPIPE] = "EPIPE"; /* 32 */
        ename[EDOM] = "EDOM"; /* 33 */
        ename[ERANGE] = "ERANGE"; /* 34 */
        ename[EDEADLK] = "EDEADLK"; /* 35 */
        ename[ENAMETOOLONG] = "ENAMETOOLONG"; /* 36 */
        ename[ENOLCK] = "ENOLCK"; /* 37 */
        ename[ENOSYS] = "ENOSYS"; /* 38 */
        ename[ENOTEMPTY] = "ENOTEMPTY"; /* 39 */
        ename[ELOOP] = "ELOOP"; /* 40 */
        ename[ENOMSG] = "ENOMSG"; /* 42 */
        ename[EIDRM] = "EIDRM"; /* 43 */
        ename[ECHRNG] = "ECHRNG"; /* 44 */
        ename[EL2NSYNC] = "EL2NSYNC"; /* 45 */
        ename[EL3HLT] = "EL3HLT"; /* 46 */
        ename[EL3RST] = "EL3RST"; /* 47 */
        ename[ELNRNG] = "ELNRNG"; /* 48 */
        ename[EUNATCH] = "EUNATCH"; /* 49 */
        ename[ENOCSI] = "ENOCSI"; /* 50 */
        ename[EL2HLT] = "EL2HLT"; /* 51 */
        ename[EBADE] = "EBADE"; /* 52 */
        ename[EBADR] = "EBADR"; /* 53 */
        ename[EXFULL] = "EXFULL"; /* 54 */
        ename[ENOANO] = "ENOANO"; /* 55 */
        ename[EBADRQC] = "EBADRQC"; /* 56 */
        ename[EBADSLT] = "EBADSLT"; /* 57 */
        ename[EBFONT] = "EBFONT"; /* 59 */
        ename[ENOSTR] = "ENOSTR"; /* 60 */
        ename[ENODATA] = "ENODATA"; /* 61 */
        ename[ETIME] = "ETIME"; /* 62 */
        ename[ENOSR] = "ENOSR"; /* 63 */
        ename[ENONET] = "ENONET"; /* 64 */
        ename[ENOPKG] = "ENOPKG"; /* 65 */
        ename[EREMOTE] = "EREMOTE"; /* 66 */
        ename[ENOLINK] = "ENOLINK"; /* 67 */
        ename[EADV] = "EADV"; /* 68 */
        ename[ESRMNT] = "ESRMNT"; /* 69 */
        ename[ECOMM] = "ECOMM"; /* 70 */
        ename[EPROTO] = "EPROTO"; /* 71 */
        ename[EMULTIHOP] = "EMULTIHOP"; /* 72 */
        ename[EDOTDOT] = "EDOTDOT"; /* 73 */
        ename[EBADMSG] = "EBADMSG"; /* 74 */
        ename[EOVERFLOW] = "EOVERFLOW"; /* 75 */
        ename[ENOTUNIQ] = "ENOTUNIQ"; /* 76 */
        ename[EBADFD] = "EBADFD"; /* 77 */
        ename[EREMCHG] = "EREMCHG"; /* 78 */
        ename[ELIBACC] = "ELIBACC"; /* 79 */
        ename[ELIBBAD] = "ELIBBAD"; /* 80 */
        ename[ELIBSCN] = "ELIBSCN"; /* 81 */
        ename[ELIBMAX] = "ELIBMAX"; /* 82 */
        ename[ELIBEXEC] = "ELIBEXEC"; /* 83 */
        ename[EILSEQ] = "EILSEQ"; /* 84 */
        ename[ERESTART] = "ERESTART"; /* 85 */
        ename[ESTRPIPE] = "ESTRPIPE"; /* 86 */
        ename[EUSERS] = "EUSERS"; /* 87 */
        ename[ENOTSOCK] = "ENOTSOCK"; /* 88 */
        ename[EDESTADDRREQ] = "EDESTADDRREQ"; /* 89 */
        ename[EMSGSIZE] = "EMSGSIZE"; /* 90 */
        ename[EPROTOTYPE] = "EPROTOTYPE"; /* 91 */
        ename[ENOPROTOOPT] = "ENOPROTOOPT"; /* 92 */
        ename[EPROTONOSUPPORT] = "EPROTONOSUPPORT"; /* 93 */
        ename[ESOCKTNOSUPPORT] = "ESOCKTNOSUPPORT"; /* 94 */
        ename[EOPNOTSUPP] = "EOPNOTSUPP"; /* 95 */
        ename[EPFNOSUPPORT] = "EPFNOSUPPORT"; /* 96 */
        ename[EAFNOSUPPORT] = "EAFNOSUPPORT"; /* 97 */
        ename[EADDRINUSE] = "EADDRINUSE"; /* 98 */
        ename[EADDRNOTAVAIL] = "EADDRNOTAVAIL"; /* 99 */
        ename[ENETDOWN] = "ENETDOWN"; /* 100 */
        ename[ENETUNREACH] = "ENETUNREACH"; /* 101 */
        ename[ENETRESET] = "ENETRESET"; /* 102 */
        ename[ECONNABORTED] = "ECONNABORTED"; /* 103 */
        ename[ECONNRESET] = "ECONNRESET"; /* 104 */
        ename[ENOBUFS] = "ENOBUFS"; /* 105 */
        ename[EISCONN] = "EISCONN"; /* 106 */
        ename[ENOTCONN] = "ENOTCONN"; /* 107 */
        ename[ESHUTDOWN] = "ESHUTDOWN"; /* 108 */
        ename[ETOOMANYREFS] = "ETOOMANYREFS"; /* 109 */
        ename[ETIMEDOUT] = "ETIMEDOUT"; /* 110 */
        ename[ECONNREFUSED] = "ECONNREFUSED"; /* 111 */
        ename[EHOSTDOWN] = "EHOSTDOWN"; /* 112 */
        ename[EHOSTUNREACH] = "EHOSTUNREACH"; /* 113 */
        ename[EALREADY] = "EALREADY"; /* 114 */
        ename[EINPROGRESS] = "EINPROGRESS"; /* 115 */
        ename[ESTALE] = "ESTALE"; /* 116 */
        ename[EUCLEAN] = "EUCLEAN"; /* 117 */
        ename[ENOTNAM] = "ENOTNAM"; /* 118 */
        ename[ENAVAIL] = "ENAVAIL"; /* 119 */
        ename[EISNAM] = "EISNAM"; /* 120 */
        ename[EREMOTEIO] = "EREMOTEIO"; /* 121 */
        ename[EDQUOT] = "EDQUOT"; /* 122 */
        ename[ENOMEDIUM] = "ENOMEDIUM"; /* 123 */
        ename[EMEDIUMTYPE] = "EMEDIUMTYPE"; /* 124 */
        ename[ECANCELED] = "ECANCELED"; /* 125 */
        ename[ENOKEY] = "ENOKEY"; /* 126 */
        ename[EKEYEXPIRED] = "EKEYEXPIRED"; /* 127 */
        ename[EKEYREVOKED] = "EKEYREVOKED"; /* 128 */
        ename[EKEYREJECTED] = "EKEYREJECTED"; /* 129 */
        ename[EOWNERDEAD] = "EOWNERDEAD"; /* 130 */
        ename[ENOTRECOVERABLE] = "ENOTRECOVERABLE"; /* 131 */
        ename[ERFKILL] = "ERFKILL"; /* 132 */
        ename[EHWPOISON] = "EHWPOISON"; /* 133 */
}
