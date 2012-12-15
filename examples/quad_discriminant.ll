; ModuleID = 'main_module'

@0 = private unnamed_addr constant [9 x i8] c"No Roots\00"
@1 = private unnamed_addr constant [3 x i8] c"%s\00"
@2 = private unnamed_addr constant [21 x i8] c"Discriminant is zero\00"
@3 = private unnamed_addr constant [3 x i8] c"%s\00"
@4 = private unnamed_addr constant [24 x i8] c"Two roots are available\00"
@5 = private unnamed_addr constant [3 x i8] c"%s\00"

define double @quad(double %A, double %B, double %C) {
func:
  %0 = alloca double
  %A1 = alloca double
  store double %A, double* %A1
  %B2 = alloca double
  store double %B, double* %B2
  %C3 = alloca double
  store double %C, double* %C3
  %N = alloca double
  %1 = load double* %B2
  %2 = load double* %B2
  %var = fmul double %1, %2
  %3 = load double* %A1
  %4 = load double* %C3
  %var4 = fmul double %3, %4
  %var5 = fmul double 4.000000e+00, %var4
  %var6 = fsub double %var, %var5
  store double %var6, double* %N
  %5 = load double* %N
  %var7 = fcmp ult double %5, 0.000000e+00
  %ifcond = icmp eq i1 %var7, true
  br i1 %ifcond, label %then, label %else

exit:                                             ; preds = %merge11, %else10, %then9, %then
  %6 = load double* %0
  ret double %6

then:                                             ; preds = %func
  store double 0.000000e+00, double* %0
  br label %exit

else:                                             ; preds = %func
  %7 = load double* %N
  %var123 = fcmp oeq double %7, 0.000000e+00
  %ifcond8 = icmp eq i1 %var123, true
  br i1 %ifcond8, label %then9, label %else10

then9:                                            ; preds = %else
  store double 1.000000e+00, double* %0
  br label %exit

else10:                                           ; preds = %else
  store double 2.000000e+00, double* %0
  br label %exit

merge:                                            ; No predecessors!
  br label %merge11

merge11:                                          ; preds = %merge
  store double 0.000000e+00, double* %0
  br label %exit
}

define i1 @main() {
func:
  %0 = alloca i1
  %D = alloca double
  %callfunc = call double @quad(double 1.000000e+00, double -6.000000e+00, double 9.000000e+00)
  store double %callfunc, double* %D
  %1 = load double* %D
  %var123 = fcmp oeq double %1, 0.000000e+00
  %ifcond = icmp eq i1 %var123, true
  br i1 %ifcond, label %then, label %else

exit:                                             ; preds = %merge5
  %2 = load i1* %0
  ret i1 %2

then:                                             ; preds = %func
  %3 = call i32 (i8*, i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8]* @0, i32 0, i32 0))
  br label %merge5

else:                                             ; preds = %func
  %4 = load double* %D
  %var1231 = fcmp oeq double %4, 1.000000e+00
  %ifcond2 = icmp eq i1 %var1231, true
  br i1 %ifcond2, label %then3, label %else4

then3:                                            ; preds = %else
  %5 = call i32 (i8*, i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @3, i32 0, i32 0), i8* getelementptr inbounds ([21 x i8]* @2, i32 0, i32 0))
  br label %merge

else4:                                            ; preds = %else
  %6 = call i32 (i8*, i8*, ...)* @printf(i8* getelementptr inbounds ([3 x i8]* @5, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8]* @4, i32 0, i32 0))
  br label %merge

merge:                                            ; preds = %else4, %then3
  br label %merge5

merge5:                                           ; preds = %merge, %then
  store i1 true, i1* %0
  br label %exit
}

declare i32 @printf(i8*, i8*, ...)
