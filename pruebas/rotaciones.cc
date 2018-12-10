for (int i = -5; i <= 0; i++) {
  x = Size * cos(M_PI * i / 10);
  y = Size * sin(M_PI * i / 10);
  if (x > 0) perfil_aux.push_back(_vertex3f(x, y, 0));
}

for (int i = 0; i >= -5; i--) {
  x = 0.9 * cos(M_PI * i / 10);
  y = 0.9 * sin(M_PI * i / 10);
  if (x > 0) perfil_aux.push_back(_vertex3f(x, y, 0));
}