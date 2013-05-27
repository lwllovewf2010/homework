; ModuleID = 'krnoops.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"

;Size of a Pointer(p): 64, ABI:64, PRE:64
;Size of an Integer(i1): 1, ABI:8, PRE:8
;Size of an Integer(i8): 8, ABI:8, PRE:8
;Size of an Integer(i16): 16, ABI:16, PRE:16
;Size of an Integer(i32): 32, ABI:32, PRE:32
;Size of an Integer(i64): 64, ABI:64, PRE:64
;Size of a Floating Point Type(f32): 32, ABI:32, PRE:32
;Size of a Floating Point Type(f64): 64, ABI:64, PRE:64
;Size of a Vector Type(v64): 64, ABI:64, PRE:64
;Size of a Vector Type(v128): 128, ABI:128, PRE:128
;The alignment for an aggregate type: 0, ABI:0, PRE:64
;The alignment of a stack object: 0, ABI:64, PRE:64
;Size of a Floating Point Type(f80): 80, ABI:128, PRE:128
;A set of native integer width for the target CPU in bits: 8, 16, 43, 64
;Natural alignment of the stack in bits: 128

target triple = "x86_64-apple-macosx10.8.0"

%struct.__sFILE = type { i8*, i32, i32, i16, i16, %struct.__sbuf, i32, i8*, i32 (i8*)*, i32 (i8*, i8*, i32)*, i64 (i8*, i64, i32)*, i32 (i8*, i8*, i32)*, %struct.__sbuf, %struct.__sFILEX*, i32, [3 x i8], [1 x i8], %struct.__sbuf, i32, i64 }
%struct.__sbuf = type { i8*, i32 }
%struct.__sFILEX = type opaque

@__stderrp = external global %struct.__sFILE*

@.str = private unnamed_addr constant [20 x i8] c"krnoops [FILENAME]\0A\00", align 1
@.str1 = private unnamed_addr constant [22 x i8] c"Cannot open file: %s\0A\00", align 1

; private, only directly accessible by objects in the current module

; unnamed_addr, indicates that the address is not significant, only the content.
; Constants marked like this can be merged with other constants if they have the same initializer.
; Note that a constant with significant address can be meraged with a unnamed_addr constant,
; the result being a constant whose address is significant.

@buf = common global i8* null, align 8
; common linkage is most similar to "weak" linkage, but they are used for tentative definitions in C,
; Such as "int X;" at global scope. Symbol with "common" linkage are merged in the same way as weak
; symbols, and they may not be deleted if unreferenced. common symbols may not have an explicit section,
; must have a zero initializer, and may not be marked 'constant'. Functions and aliases may not have
; common linkage

; global, global variable
; global variable defines regions of memory allocated at compilation time instead of run-time.
; Global variables may optionally be initialized, may have an explicit section to be placed in,
; and may have an optional explicit alignment specified.


@.str2 = private unnamed_addr constant [21 x i8] c"Out of memory (buf)\0A\00", align 1
@.str3 = private unnamed_addr constant [37 x i8] c"Cannot read full length of the file\0A\00", align 1
@tmpbuf = common global i8* null, align 8
@.str4 = private unnamed_addr constant [24 x i8] c"Out of memory (tmpbuf)\0A\00", align 1
@inst = common global [512 x i8] zeroinitializer, align 16
@.str5 = private unnamed_addr constant [25 x i8] c"Oops Code @ address: %p\0A\00", align 1

define void @help() nounwind uwtable ssp {
entry:
  %0 = load %struct.__sFILE** @__stderrp, align 8
  %call = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %0, i8* getelementptr inbounds ([20 x i8]* @.str, i32 0, i32 0))
  ret void
}

declare i32 @fprintf(%struct.__sFILE*, i8*, ...)

define i32 @checkHexAscii(i8 signext %c) nounwind uwtable ssp {
entry:
  %retval = alloca i32, align 4
  %c.addr = alloca i8, align 1
  store i8 %c, i8* %c.addr, align 1
  %0 = load i8* %c.addr, align 1
  %conv = sext i8 %0 to i32
  %cmp = icmp sge i32 %conv, 48
  br i1 %cmp, label %land.lhs.true, label %lor.lhs.false

land.lhs.true:                                    ; preds = %entry
  %1 = load i8* %c.addr, align 1
  %conv2 = sext i8 %1 to i32
  %cmp3 = icmp sle i32 %conv2, 57
  br i1 %cmp3, label %if.then, label %lor.lhs.false

lor.lhs.false:                                    ; preds = %land.lhs.true, %entry
  %2 = load i8* %c.addr, align 1
  %conv5 = sext i8 %2 to i32
  %cmp6 = icmp sge i32 %conv5, 65
  br i1 %cmp6, label %land.lhs.true8, label %lor.lhs.false12

land.lhs.true8:                                   ; preds = %lor.lhs.false
  %3 = load i8* %c.addr, align 1
  %conv9 = sext i8 %3 to i32
  %cmp10 = icmp sle i32 %conv9, 70
  br i1 %cmp10, label %if.then, label %lor.lhs.false12

lor.lhs.false12:                                  ; preds = %land.lhs.true8, %lor.lhs.false
  %4 = load i8* %c.addr, align 1
  %conv13 = sext i8 %4 to i32
  %cmp14 = icmp sge i32 %conv13, 97
  br i1 %cmp14, label %land.lhs.true16, label %if.end

land.lhs.true16:                                  ; preds = %lor.lhs.false12
  %5 = load i8* %c.addr, align 1
  %conv17 = sext i8 %5 to i32
  %cmp18 = icmp sle i32 %conv17, 102
  br i1 %cmp18, label %if.then, label %if.end

if.then:                                          ; preds = %land.lhs.true16, %land.lhs.true8, %land.lhs.true
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %land.lhs.true16, %lor.lhs.false12
  store i32 1, i32* %retval
  br label %return

return:                                           ; preds = %if.end, %if.then
  %6 = load i32* %retval
  ret i32 %6
}

define i32 @main(i32 %argc, i8** %argv) nounwind uwtable ssp {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %fd = alloca i32, align 4
  %fsz = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %tmp = alloca [3 x i8], align 1
  %c = alloca i8, align 1
  store i32 0, i32* %retval
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %0 = load i32* %argc.addr, align 4
  %cmp = icmp ne i32 %0, 2
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  call void @help()
  store i32 0, i32* %retval
  br label %return

if.end:                                           ; preds = %entry
  %1 = load i8*** %argv.addr, align 8
  %arrayidx = getelementptr inbounds i8** %1, i64 1
  %2 = load i8** %arrayidx, align 8
  %call = call i32 (i8*, i32, ...)* @"\01_open"(i8* %2, i32 2)
  store i32 %call, i32* %fd, align 4
  %3 = load i32* %fd, align 4
  %cmp2 = icmp slt i32 %3, 0
  br i1 %cmp2, label %if.then3, label %if.end6

if.then3:                                         ; preds = %if.end
  %4 = load %struct.__sFILE** @__stderrp, align 8
  %5 = load i8*** %argv.addr, align 8
  %arrayidx4 = getelementptr inbounds i8** %5, i64 1
  %6 = load i8** %arrayidx4, align 8
  %call5 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %4, i8* getelementptr inbounds ([22 x i8]* @.str1, i32 0, i32 0), i8* %6)
  store i32 -1, i32* %retval
  br label %return

if.end6:                                          ; preds = %if.end
  %7 = load i32* %fd, align 4
  %call7 = call i64 @lseek(i32 %7, i64 0, i32 2)
  %conv = trunc i64 %call7 to i32
  store i32 %conv, i32* %fsz, align 4
  %8 = load i32* %fd, align 4
  %call8 = call i64 @lseek(i32 %8, i64 0, i32 0)
  %9 = load i32* %fsz, align 4
  %conv9 = sext i32 %9 to i64
  %call10 = call i8* @malloc(i64 %conv9)
  store i8* %call10, i8** @buf, align 8
  %10 = load i8** @buf, align 8
  %tobool = icmp ne i8* %10, null
  br i1 %tobool, label %if.end13, label %if.then11

if.then11:                                        ; preds = %if.end6
  %11 = load %struct.__sFILE** @__stderrp, align 8
  %call12 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %11, i8* getelementptr inbounds ([21 x i8]* @.str2, i32 0, i32 0))
  store i32 -1, i32* %retval
  br label %return

if.end13:                                         ; preds = %if.end6
  %12 = load i8** @buf, align 8
  %13 = call i64 @llvm.objectsize.i64(i8* %12, i1 false)
  %cmp14 = icmp ne i64 %13, -1
  br i1 %cmp14, label %cond.true, label %cond.false

cond.true:                                        ; preds = %if.end13
  %14 = load i8** @buf, align 8
  %15 = load i32* %fsz, align 4
  %conv16 = sext i32 %15 to i64
  %16 = load i8** @buf, align 8
  %17 = call i64 @llvm.objectsize.i64(i8* %16, i1 false)
  %call17 = call i8* @__memset_chk(i8* %14, i32 0, i64 %conv16, i64 %17) nounwind
  br label %cond.end

cond.false:                                       ; preds = %if.end13
  %18 = load i8** @buf, align 8
  %19 = load i32* %fsz, align 4
  %conv18 = sext i32 %19 to i64
  %call19 = call i8* @__inline_memset_chk(i8* %18, i32 0, i64 %conv18)
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i8* [ %call17, %cond.true ], [ %call19, %cond.false ]
  %20 = load i32* %fd, align 4
  %21 = load i8** @buf, align 8
  %22 = load i32* %fsz, align 4
  %conv20 = sext i32 %22 to i64
  %call21 = call i64 @"\01_read"(i32 %20, i8* %21, i64 %conv20)
  %23 = load i32* %fsz, align 4
  %conv22 = sext i32 %23 to i64
  %cmp23 = icmp ne i64 %call21, %conv22
  br i1 %cmp23, label %if.then25, label %if.end28

if.then25:                                        ; preds = %cond.end
  %24 = load %struct.__sFILE** @__stderrp, align 8
  %call26 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %24, i8* getelementptr inbounds ([37 x i8]* @.str3, i32 0, i32 0))
  %25 = load i32* %fd, align 4
  %call27 = call i32 @"\01_close"(i32 %25)
  store i32 -1, i32* %retval
  br label %return

if.end28:                                         ; preds = %cond.end
  %26 = load i32* %fd, align 4
  %call29 = call i32 @"\01_close"(i32 %26)
  %27 = load i32* %fsz, align 4
  %conv30 = sext i32 %27 to i64
  %call31 = call i8* @malloc(i64 %conv30)
  store i8* %call31, i8** @tmpbuf, align 8
  %28 = load i8** @tmpbuf, align 8
  %tobool32 = icmp ne i8* %28, null
  br i1 %tobool32, label %if.end35, label %if.then33

if.then33:                                        ; preds = %if.end28
  %29 = load %struct.__sFILE** @__stderrp, align 8
  %call34 = call i32 (%struct.__sFILE*, i8*, ...)* @fprintf(%struct.__sFILE* %29, i8* getelementptr inbounds ([24 x i8]* @.str4, i32 0, i32 0))
  store i32 -1, i32* %retval
  br label %return

if.end35:                                         ; preds = %if.end28
  %arrayidx36 = getelementptr inbounds [3 x i8]* %tmp, i32 0, i64 2
  store i8 0, i8* %arrayidx36, align 1
  store i32 0, i32* %i, align 4
  store i32 0, i32* %j, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end35
  %30 = load i32* %i, align 4
  %31 = load i32* %fsz, align 4
  %cmp37 = icmp slt i32 %30, %31
  br i1 %cmp37, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %32 = load i8** @buf, align 8
  %33 = load i32* %i, align 4
  %idx.ext = sext i32 %33 to i64
  %add.ptr = getelementptr inbounds i8* %32, i64 %idx.ext
  %34 = load i8* %add.ptr, align 1
  store i8 %34, i8* %c, align 1
  %35 = load i8* %c, align 1
  %call39 = call i32 @checkHexAscii(i8 signext %35)
  %tobool40 = icmp ne i32 %call39, 0
  br i1 %tobool40, label %if.then41, label %if.end42

if.then41:                                        ; preds = %for.body
  br label %for.inc

if.end42:                                         ; preds = %for.body
  %36 = load i8* %c, align 1
  %37 = load i32* %j, align 4
  %inc = add nsw i32 %37, 1
  store i32 %inc, i32* %j, align 4
  %idxprom = sext i32 %37 to i64
  %arrayidx43 = getelementptr inbounds [3 x i8]* %tmp, i32 0, i64 %idxprom
  store i8 %36, i8* %arrayidx43, align 1
  %38 = load i32* %j, align 4
  %cmp44 = icmp sge i32 %38, 2
  br i1 %cmp44, label %if.then46, label %if.end52

if.then46:                                        ; preds = %if.end42
  store i32 0, i32* %j, align 4
  %arraydecay = getelementptr inbounds [3 x i8]* %tmp, i32 0, i32 0
  %call47 = call i64 @strtol(i8* %arraydecay, i8** null, i32 16)
  %and = and i64 %call47, 255
  %conv48 = trunc i64 %and to i8
  %39 = load i32* %k, align 4
  %inc49 = add nsw i32 %39, 1
  store i32 %inc49, i32* %k, align 4
  %idxprom50 = sext i32 %39 to i64
  %40 = load i8** @tmpbuf, align 8
  %arrayidx51 = getelementptr inbounds i8* %40, i64 %idxprom50
  store i8 %conv48, i8* %arrayidx51, align 1
  br label %if.end52

if.end52:                                         ; preds = %if.then46, %if.end42
  br label %for.inc

for.inc:                                          ; preds = %if.end52, %if.then41
  %41 = load i32* %i, align 4
  %inc53 = add nsw i32 %41, 1
  store i32 %inc53, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  call void @llvm.memset.p0i8.i64(i8* getelementptr inbounds ([512 x i8]* @inst, i32 0, i32 0), i8 0, i64 512, i32 16, i1 false)
  %42 = load i8** @tmpbuf, align 8
  %43 = load i32* %k, align 4
  %conv54 = sext i32 %43 to i64
  %call55 = call i8* @__memcpy_chk(i8* getelementptr inbounds ([512 x i8]* @inst, i32 0, i32 0), i8* %42, i64 %conv54, i64 512) nounwind
  %44 = load i8** @tmpbuf, align 8
  call void @free(i8* %44)
  %45 = load i8** @buf, align 8
  call void @free(i8* %45)
  %call56 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([25 x i8]* @.str5, i32 0, i32 0), i8* getelementptr inbounds ([512 x i8]* @inst, i32 0, i32 0))
  store i32 0, i32* null, align 4
  store i32 0, i32* %retval
  br label %return

return:                                           ; preds = %for.end, %if.then33, %if.then25, %if.then11, %if.then3, %if.then
  %46 = load i32* %retval
  ret i32 %46
}

declare i32 @"\01_open"(i8*, i32, ...)

declare i64 @lseek(i32, i64, i32)

declare i8* @malloc(i64)

declare i64 @llvm.objectsize.i64(i8*, i1) nounwind readnone

declare i8* @__memset_chk(i8*, i32, i64, i64) nounwind

define internal i8* @__inline_memset_chk(i8* %__dest, i32 %__val, i64 %__len) nounwind uwtable inlinehint ssp {
entry:
  %__dest.addr = alloca i8*, align 8
  %__val.addr = alloca i32, align 4
  %__len.addr = alloca i64, align 8
  store i8* %__dest, i8** %__dest.addr, align 8
  store i32 %__val, i32* %__val.addr, align 4
  store i64 %__len, i64* %__len.addr, align 8
  %0 = load i8** %__dest.addr, align 8
  %1 = load i32* %__val.addr, align 4
  %2 = load i64* %__len.addr, align 8
  %3 = load i8** %__dest.addr, align 8
  %4 = call i64 @llvm.objectsize.i64(i8* %3, i1 false)
  %call = call i8* @__memset_chk(i8* %0, i32 %1, i64 %2, i64 %4) nounwind
  ret i8* %call
}

declare i64 @"\01_read"(i32, i8*, i64)

declare i32 @"\01_close"(i32)

declare i64 @strtol(i8*, i8**, i32)

declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) nounwind

declare i8* @__memcpy_chk(i8*, i8*, i64, i64) nounwind

declare void @free(i8*)

declare i32 @printf(i8*, ...)
