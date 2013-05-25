; ModuleID = '<stdin>'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.8.0"

@.str = private unnamed_addr constant [15 x i8] c"[%d][%d] = %d\0A\00", align 1

define i32 @main(i32 %argc, i8** %argv) nounwind uwtable ssp {
entry:
  %tst = alloca [50 x [50 x i32]], align 16
  br label %for.cond

for.cond:                                         ; preds = %for.inc13, %entry
  %indvar1 = phi i64 [ %indvar.next2, %for.inc13 ], [ 0, %entry ]
  %i.0 = trunc i64 %indvar1 to i32
  %0 = mul i64 %indvar1, 17400
  %exitcond3 = icmp ne i64 %indvar1, 50
  br i1 %exitcond3, label %for.body, label %for.end15

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %indvar = phi i64 [ %indvar.next, %for.inc ], [ 0, %for.body ]
  %arrayidx8 = getelementptr [50 x [50 x i32]]* %tst, i64 0, i64 %indvar1, i64 %indvar
  %j.0 = trunc i64 %indvar to i32
  %1 = mul i64 %0, %indvar
  %mul6 = trunc i64 %1 to i32
  %exitcond = icmp ne i64 %indvar, 50
  br i1 %exitcond, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  store i32 %mul6, i32* %arrayidx8, align 4
  %2 = load i32* %arrayidx8, align 4
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([15 x i8]* @.str, i32 0, i32 0), i32 %i.0, i32 %j.0, i32 %2)
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %indvar.next = add i64 %indvar, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc13

for.inc13:                                        ; preds = %for.end
  %indvar.next2 = add i64 %indvar1, 1
  br label %for.cond

for.end15:                                        ; preds = %for.cond
  ret i32 0
}

declare i32 @printf(i8*, ...)
