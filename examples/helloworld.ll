; ModuleID = 'main_module'

@0 = private unnamed_addr constant [12 x i8] c"Hello World\00"
@1 = private unnamed_addr constant [3 x i8] c"%s\00"

define i1 @main() {
func:
  %0 = alloca i1
  %1 = call i32 (i8*, i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8]* @0, i32 0, i32 0))
  store i1 true, i1* %0
  br label %exit

exit:                                             ; preds = %func
  %2 = load i1* %0
  ret i1 %2
}

declare i32 @printf(i8*, i8*, ...)
