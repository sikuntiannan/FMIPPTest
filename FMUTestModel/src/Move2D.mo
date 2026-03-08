model Move2D
  // 参数
  parameter Real start_x = 0.0 "起点 x 坐标";
  parameter Real start_y = 0.0 "起点 y 坐标";
  parameter Real end_x   = 10.0 "终点 x 坐标";
  parameter Real end_y   = 10.0 "终点 y 坐标";
  parameter Real v = 1.0 "速度大小（恒为正）";
  parameter Real eps = 1e-6 "到达终点判断容差";

  // 输入
  input Boolean stop "true 表示停止，false 表示运动";

  // 输出
  output Real x "当前位置 x 坐标";
  output Real y "当前位置 y 坐标";

protected
  // 总位移向量及其长度
  Real dx_total = end_x - start_x;
  Real dy_total = end_y - start_y;
  Real dist_total = sqrt(dx_total^2 + dy_total^2);
  // 方向单位向量（当起点与终点重合时，方向为零向量）
  Real ex = if dist_total > 0 then dx_total / dist_total else 0;
  Real ey = if dist_total > 0 then dy_total / dist_total else 0;
  // 是否已到达终点（一旦到达即永久锁定）
  Boolean arrived;

initial equation
  x = start_x;
  y = start_y;
  // 初始化时判断是否已经在终点附近
  arrived = sqrt((start_x - end_x)^2 + (start_y - end_y)^2) <= eps;

equation
  // 速度由停止信号和到达标志共同控制
  der(x) = if stop or arrived then 0 else v * ex;
  der(y) = if stop or arrived then 0 else v * ey;

  // 当位置接近终点时，将 arrived 置为 true（仅触发一次）
  when sqrt((x - end_x)^2 + (y - end_y)^2) <= eps then
    arrived = true;
  end when;
end Move2D;
