## asr Makefile ##
#!/bin/bash
CC=g++
#AR=ar

#FILE_NAME=$(src)
FILE_NAME=../src/*.cpp ../src/asrdemo/*.cpp
INC_PATH= -I ../../../include\
          -I ../../../include/ASR\
          -I ../src/asrdemo/

SRC_PATH=../src
OBJ_PATH=.
TARGET=asr
LIB_PATH=../../../lib
EXTERN_PATH=../../../extern/lib
TARGET_PATH=./
CFLAGS_LIB = -std=c++11 -lpthread


CPPFLAGS1=-Wall -O0 -fPIC -g -D__LINUX__ -Wno-unknown-pragmas -D_GLIBCXX_USE_CXX11_ABI=0
CPPFLAGS= $(CPPFLAGS1) $(LIB_PATH)/libBDSpeechSDK.a $(EXTERN_PATH)/libcurl.a $(EXTERN_PATH)/libiconv.a $(EXTERN_PATH)/libz.a $(EXTERN_PATH)/libssl.a $(EXTERN_PATH)/libcrypto.a $(EXTERN_PATH)/libuuid.a -lrt -ldl -lpthread
SRC_FILE=$(shell cd $(SRC_PATH)&&echo *.cpp)
SRC:=$(foreach n,$(SRC_FILE),$(SRC_PATH)/$(n))

OBJ_FILE=$(SRC_FILE:.cpp=.o)
OBJ:=$(foreach n,$(OBJ_FILE),$(OBJ_PATH)/$(n))
	          
DEP_FILE=$(SRC_FILE:.cpp=.d)
DEP:=$(foreach n,$(DEP_FILE),$(OBJ_PATH)/$(n))

$(TARGET):$(SRC)
	$(CC) -o $(TARGET)  ./$(FILE_NAME) $(INC_PATH) $(CPPFLAGS) $(CFLAGS_LIB)
	test -d "wav" || mkdir -p "wav"
	
clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)
	-rm -rf BDS_Ident.dat *.log 
	-rm -rf wav




