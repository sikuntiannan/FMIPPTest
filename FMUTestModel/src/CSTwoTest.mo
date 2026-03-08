model CSTwoTest
  type Mode = enumeration(small, medium, large);
  // 参数
  parameter Real p_real = 1.0;
  parameter Integer p_int = 5;
  parameter Boolean p_bool = true;
  parameter Mode p_enum = Mode.medium;

  // 输入
  input Real u_real;
  input Integer u_int;
  input Boolean u_bool;
  input Mode u_enum;

  // 输出
  output Real y_real;
  output Integer y_int;
  output Boolean y_bool;
  output Mode y_enum;

  // 内部逻辑（示意）
equation
  y_real = u_real * p_real;
  y_int = u_int + p_int;
  y_bool = u_bool and p_bool;
  y_enum = if u_enum == Mode.small then Mode.medium else Mode.large;
end CSTwoTest;
