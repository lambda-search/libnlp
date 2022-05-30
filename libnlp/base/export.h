
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/

#pragma once

#if defined(LIBNLP_BUILT_AS_STATIC) || !defined(_WIN32)
#define LIBNLP_EXPORT
#define LIBNLP_NO_EXPORT
#else // if defined(LIBNLP_BUILT_AS_STATIC) || !defined(_WIN32)
#ifndef LIBNLP_EXPORT
#ifdef NLPCC_EXPORTS

/* We are building this library */
#define LIBNLP_EXPORT __declspec(dllexport)
#else // ifdef NLPCC_EXPORTS

/* We are using this library */
#define LIBNLP_EXPORT __declspec(dllimport)
#endif // ifdef NLPCC_EXPORTS
#endif // ifndef LIBNLP_EXPORT

#ifndef LIBNLP_NO_EXPORT
#define LIBNLP_NO_EXPORT
#endif // ifndef LIBNLP_NO_EXPORT
#endif // if defined(LIBNLP_BUILT_AS_STATIC) || !defined(_WIN32)
