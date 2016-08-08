#include "PrettyLogger.h"

/*
 * Copyright (c) 2016 Particle Industries, Inc., Hi-Z Labs, LLC  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cinttypes>
#include <cstdio>

namespace {

const char* extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1; // Skip return type
        } else if (*s == '(') {
            break; // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

} // namespace

//void PrettyStreamLogHandler::padField(const char* text, const unsigned width) {
//	unsigned len = strlen(text);
//	for(unsigned i = len; i < width; i++)
//		write(" ");
//}

static void padField(char* buf, size_t bufSize, const char* text, const unsigned width) {
	int len = (int)width - (int)strlen(text);
	len = max(0, len);
	int start = strlen(buf);
	int maxLen = bufSize-start-1;
	len = min(len, maxLen);
	memset(buf+start, ' ', len);
	buf[start+len+1] = 0;
}

//
//void PrettyStreamLogHandler::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
//	if(!HAL_IsISR())
//	{
//		size_t sizeof_buf = 50;
//		char* buf = (char*)malloc(sizeof_buf);
//		if(buf) {
//			logMessage(buf, sizeof_buf, msg, level, category, attr);
//			free(buf);
//		} else
//			logMessageStack(msg, level, category, attr);
//	} else
//		logMessageStack(msg, level, category, attr);
//}
//
//void PrettyStreamLogHandler::logMessageStack(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
//	char buf[40];
//	logMessage(buf, sizeof(buf), msg, level, category, attr);
//}

// spark::StreamLogHandler
//void PrettyStreamLogHandler::logMessage(char* buf, size_t sizeof_buf, const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
//
//	write(Time.timeStr().c_str());
//
//    // Timestamp
//    if (attr.has_time) {
//    	ATOMIC_BLOCK()
//		{
//    		snprintf(buf, sizeof_buf, "%8u.%03u", (unsigned)attr.time/1000, (unsigned)attr.time%1000);
//		}
//        write(buf);
//    } else {
//    	padField("", 11);
//    }
//
//    write(" ");
//
//    // Level
//    const char* lvlName = levelName(level);
//    write(lvlName);
//    padField(lvlName, 5);
//    write(" ");
//
//    // Category
//    if (category) {
//        write(category);
//        padField(category, 30);
//    }
//    // Source file
//    if (attr.has_file) {
//        // Strip directory path
//        const char *s = extractFileName(attr.file);
//        if (attr.has_line) {
//        	ATOMIC_BLOCK()
//			{
//        		snprintf(buf, sizeof_buf, "%s:%d", s, attr.line); // Line number
//			}
//            padField(buf, 30);
//            write(buf);
//        }
////        if (attr.has_function) {
////            write(" ");
////        } else {
////            write(" ");
////        }
//    } else {
//    	padField("", 30);
//    }
//
//    write(" ");
//
//    // Function name
//    if (attr.has_function) {
//        // Strip argument and return types for better readability
//        size_t n = 0;
//        const char *s = extractFuncName(attr.function, &n);
//        memset(buf, 0, sizeof_buf);
//        strncpy(buf, s, n);
//        strncpy(buf+n, "()", sizeof_buf-n);
//        write(buf);
//        padField(buf, 40);
//
//    } else {
//    	padField("", 40);
//    }
//    write(" ");
//
//    // Additional attributes
//    if (attr.has_code || attr.has_details) {
//        write(" [");
//        if (attr.has_code) {
//            write("code");
//            write(" = ");
//            ATOMIC_BLOCK()
//            {
//            	snprintf(buf, sizeof_buf, "%" PRIiPTR, attr.code);
//            }
//            write(buf);
//        }
//        if (attr.has_details) {
//            if (attr.has_code) {
//                write(", ");
//            }
//            write("details");
//            write(" = ");
//            write(attr.details);
//        }
//        write("]");
//    }
//    write(" ");
//
////    write("\r\n> ");
//    // Message
//    if (msg) {
//        write(msg);
//    }
//    write("\r\n");
//}

//void PrettyStreamLogHandler::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
//	const unsigned bufSize = 256;
//	char* buf = (char*)malloc(bufSize);
//	if(!buf) {
//		write("LOGGER: Error allocating buffer \r\n");
//		write("   ");
//		write(msg);
//		write("\r\n");
//		return;
//	}
//
//	char* s = buf;
//
//	ATOMIC_BLOCK()
//	{
//		s = s + snprintf(s, bufSize, "%s %8u.%03u %5s %30s %-30s %4d %s",
//				Time.timeStr().c_str(),
//				attr.has_time? (unsigned)attr.time/1000 : 0u,
//				attr.has_time? (unsigned)attr.time%1000 : 0u,
//				levelName(level),
//				category ? category : "");
//	}
//
//
//
//	write(Time.timeStr().c_str());
//
//    // Timestamp
//    if (attr.has_time) {
//    	ATOMIC_BLOCK()
//		{
//    		snprintf(buf, sizeof_buf, "%8u.%03u", (unsigned)attr.time/1000, (unsigned)attr.time%1000);
//		}
//        write(buf);
//    } else {
//    	padField("", 11);
//    }
//
//    write(" ");
//
//    // Level
//    const char* lvlName = levelName(level);
//    write(lvlName);
//    padField(lvlName, 5);
//    write(" ");
//
//    // Category
//    if (category) {
//        write(category);
//        padField(category, 30);
//    }
//    // Source file
//    if (attr.has_file) {
//        // Strip directory path
//        const char *s = extractFileName(attr.file);
//        if (attr.has_line) {
//        	ATOMIC_BLOCK()
//			{
//        		snprintf(buf, sizeof_buf, "%s:%d", s, attr.line); // Line number
//			}
//            padField(buf, 30);
//            write(buf);
//        }
////        if (attr.has_function) {
////            write(" ");
////        } else {
////            write(" ");
////        }
//    } else {
//    	padField("", 30);
//    }
//
//    write(" ");
//
//    // Function name
//    if (attr.has_function) {
//        // Strip argument and return types for better readability
//        size_t n = 0;
//        const char *s = extractFuncName(attr.function, &n);
//        memset(buf, 0, sizeof_buf);
//        strncpy(buf, s, n);
//        strncpy(buf+n, "()", sizeof_buf-n);
//        write(buf);
//        padField(buf, 40);
//
//    } else {
//    	padField("", 40);
//    }
//    write(" ");
//
//    // Additional attributes
//    if (attr.has_code || attr.has_details) {
//        write(" [");
//        if (attr.has_code) {
//            write("code");
//            write(" = ");
//            ATOMIC_BLOCK()
//            {
//            	snprintf(buf, sizeof_buf, "%" PRIiPTR, attr.code);
//            }
//            write(buf);
//        }
//        if (attr.has_details) {
//            if (attr.has_code) {
//                write(", ");
//            }
//            write("details");
//            write(" = ");
//            write(attr.details);
//        }
//        write("]");
//    }
//    write(" ");
//
////    write("\r\n> ");
//    // Message
//    if (msg) {
//        write(msg);
//    }
//    write("\r\n");
//}

// spark::StreamLogHandler
void PrettyStreamLogHandler::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
//	const unsigned sizeof_output = 256;
//	const unsigned sizeof_buf = 50;
//	char* output = (char*)malloc(sizeof_output);
//	char* buf = (char*)malloc(sizeof_buf);
//	if(!output || !buf) {
//		write("LOGGER: Error allocating buffers \r\n");
//		write("   ");
//		write(msg);
//		write("\r\n");
//		if(output)
//			free(output);
//		if(buf)
//			free(buf);
//		return;
//	}

	if(!HAL_IsISR())
	{
		String filename = "";
		String function = attr.has_function ? attr.function : "";
		int endOfFunction = function.indexOf('(');
		int startOfFunction = function.indexOf(' ');
		if(endOfFunction >= 0) {
			function.remove(endOfFunction);
			function.concat("()");

		}

		if(startOfFunction >= 0) {
			function = function.substring(startOfFunction+1);
		}

		if (attr.has_file) {
			// Strip directory path
			const char *s = extractFileName(attr.file);
			filename = String::format("%30s:%-5d", s, attr.has_line ? attr.line : 0); // Line number
		}

		String attribs = "";

		// Additional attributes
		if (attr.has_code || attr.has_details) {
			attribs = "[";
			if (attr.has_code) {
				attribs.concat("code = ");
				attribs.concat(String::format("%" PRIiPTR, attr.code));
			}
			if (attr.has_details) {
				if (attr.has_code) {
					attribs.concat(", ");
				}
				attribs.concat("details = ");
				attribs.concat(attr.details);
			}
			attribs.concat("]");
		}

		String output = String::format("%s %8u.%03u %-5s %-20s %36s %-40s %s %s\r\n",
			Time.timeStr().c_str(),
			attr.has_time? (unsigned)attr.time/1000 : 0,
			attr.has_time? (unsigned)attr.time%1000 : 0,
			levelName(level),
			category? category : "",
			filename.c_str(),
			function.c_str(),
			msg,
			attribs.c_str());
		SINGLE_THREADED_BLOCK()
		{
			write(output.c_str());
		}
	}
}
