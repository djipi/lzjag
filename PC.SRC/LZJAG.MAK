# Microsoft Visual C++ generated build script - Do not modify

PROJ = LZJAG
DEBUG = 0
PROGTYPE = 6
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\DEV\JAGUAR\TOOLDEV\LZSS\LZJAG\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = MAIN.C      
FIRSTCPP =             
RC = rc
CFLAGS_D_DEXE = /nologo /G2 /W3 /Zi /AH /Od /D "_DEBUG" /D "_DOS" /FR /Fd"LZJAG.PDB"
CFLAGS_R_DEXE = /nologo /Gs /G2 /W3 /AH /Ox /D "NDEBUG" /D "_DOS" 
LFLAGS_D_DEXE = /NOLOGO /NOI /STACK:5120 /ONERROR:NOEXE /CO 
LFLAGS_R_DEXE = /NOLOGO /NOI /STACK:16384 /ONERROR:NOEXE 
LIBS_D_DEXE = oldnames llibce 
LIBS_R_DEXE = oldnames llibce 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_DEXE)
LFLAGS = $(LFLAGS_D_DEXE)
LIBS = $(LIBS_D_DEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_DEXE)
LFLAGS = $(LFLAGS_R_DEXE)
LIBS = $(LIBS_R_DEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = MAIN.SBR \
		LZSS.SBR \
		BITIO.SBR \
		ERRHAND.SBR


MAIN_DEP = c:\dev\jaguar\tooldev\lzss\lzjag\bitio.h


LZSS_DEP = c:\dev\jaguar\tooldev\lzss\lzjag\bitio.h


BITIO_DEP = c:\dev\jaguar\tooldev\lzss\lzjag\bitio.h \
	c:\dev\jaguar\tooldev\lzss\lzjag\errhand.h


ERRHAND_DEP = c:\dev\jaguar\tooldev\lzss\lzjag\errhand.h


all:	$(PROJ).EXE

MAIN.OBJ:	MAIN.C $(MAIN_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c MAIN.C

LZSS.OBJ:	LZSS.C $(LZSS_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c LZSS.C

BITIO.OBJ:	BITIO.C $(BITIO_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c BITIO.C

ERRHAND.OBJ:	ERRHAND.C $(ERRHAND_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c ERRHAND.C

$(PROJ).EXE::	MAIN.OBJ LZSS.OBJ BITIO.OBJ ERRHAND.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
MAIN.OBJ +
LZSS.OBJ +
BITIO.OBJ +
ERRHAND.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\dev\msvc\lib\+
c:\dev\msvc\mfc\lib\+
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
