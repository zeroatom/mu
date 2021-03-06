/**
 * @file config.h
 * Copyright (C) 

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 * @brief 一些配置宏
 * @author lee, china2011long@163.com
 * @version 1.0.0
 * @date 2017-01-13
 */
#ifndef _BASE_CONFIG_H_
#define _BASE_CONFIG_H_

// 根据指针删除内存
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if((x) != NULL) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if ((x) != NULL) { delete[] (x); (x) = NULL;  }
#endif

#endif

