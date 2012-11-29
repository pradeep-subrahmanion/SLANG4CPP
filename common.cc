//
//  common.cc
//  TestCPP
//
//  Created by Pradeep Subrahmanion on 29/11/12.
//  Copyright (c) 2012 Pradeep Subrahmanion. All rights reserved.
//

#include "common.h"

void exit_with_message(const char *err_msg) {
  
  std::cout << "\n" << err_msg << "\n";
  exit(0);
}