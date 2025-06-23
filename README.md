# 🎯 Pop 'n' Shoot

**Pop 'n' Shoot** is a colorful and fun **Bubble Shooting Game** written in **C**, using classic console graphics (like `<graphics.h>`). Inspired by the popular genre of bubble shooters, this version brings the arcade experience to the terminal with a minimalist design and mouse-powered interactivity.

## 🕹️ Game Overview

Move the shooter with your mouse, aim carefully, and match bullet colors with the bubbles to pop them! But there's a twist — only **color-matched** bullets can burst the bubbles. So time your **reloads** (right-click), aim with precision (mouse), and **fire** (left-click) at the right target.

> A console game with arcade-style flair!

---

## ✨ Features

- 🎨 **Color-matching mechanics**
- 🖱️ **Mouse-controlled shooter**
- 🔫 **Left-click to shoot, Right-click to reload**
- 💥 **Bubble burst effects**
- 🧠 **Custom-built game loop and graphics rendering**
- 🧹 **Dynamic memory usage for bullets and bubbles**

---

## ⚙️ Core Functions

The game logic is structured around the following key functions:

```c
void Item_display(void);
void create_shooter(int, int, int);
void getmousecord(int *, int *, int *);
void fire_bullet(void);
void initframe(void);
void loading(void);
void throw_ballon(void);
int bullet_display(Bullet *);
Ballon* ballon_display(Ballon *);
void bullet_design(Bullet *);
Ballon* remove_ballon(Ballon *, int);
Bullet* remove_bullet(Bullet *);
void chk_bullet_hit(void);
void ballon_wasted(void);
void reload(int);
