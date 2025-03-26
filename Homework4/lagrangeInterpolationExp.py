import pygame
import numpy as np

# Initialize pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Lagrange vs Bezier Curve")
font = pygame.font.SysFont("Arial", 20)
clock = pygame.time.Clock()

# Colors
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 200, 0)
BLUE = (0, 0, 255)
GRAY = (220, 220, 220)
BLACK = (0, 0, 0)

# Mode state
mode = None  # 'lagrange' or 'bezier'
show_menu = False
menu_pos = (0, 0)
points = []


def lagrange_interpolate(x, x_points, y_points):
    total = 0
    n = len(x_points)
    for j in range(n):
        term = y_points[j]
        for m in range(n):
            if m != j:
                term *= (x - x_points[m]) / (x_points[j] - x_points[m])
        total += term
    return total


def bezier_interpolate(t, control_points):
    n = len(control_points) - 1
    point = np.zeros(2)
    for i, (x, y) in enumerate(control_points):
        binomial = comb(n, i)
        point[0] += binomial * ((1 - t) ** (n - i)) * (t ** i) * x
        point[1] += binomial * ((1 - t) ** (n - i)) * (t ** i) * y
    return point


def comb(n, k):
    from math import factorial
    return factorial(n) // (factorial(k) * factorial(n - k))


def draw_curve():
    if len(points) < 2 or mode is None:
        return

    if mode == 'lagrange':
        x_vals = [p[0] for p in points]
        y_vals = [p[1] for p in points]
        last_pos = None
        for x in range(0, width):
            try:
                y = lagrange_interpolate(x, x_vals, y_vals)
                if 0 <= y < height:
                    if last_pos:
                        pygame.draw.line(screen, GREEN, last_pos, (x, y), 2)
                    last_pos = (x, y)
            except ZeroDivisionError:
                continue

    elif mode == 'bezier':
        last_pos = None
        steps = 1000
        for i in range(steps + 1):
            t = i / steps
            x, y = bezier_interpolate(t, points)
            if last_pos:
                pygame.draw.line(screen, BLUE, last_pos, (x, y), 2)
            last_pos = (x, y)


def draw_menu(pos):
    options = ["Lagrange", "Bezier", "Reset"]
    w, h = 150, 90
    pygame.draw.rect(screen, GRAY, (pos[0], pos[1], w, h))
    pygame.draw.rect(screen, BLACK, (pos[0], pos[1], w, h), 2)
    for i, text in enumerate(options):
        label = font.render(text, True, BLACK)
        screen.blit(label, (pos[0] + 10, pos[1] + 10 + i * 25))


def handle_menu_click(pos):
    global mode, show_menu, points
    x, y = menu_pos
    if x <= pos[0] <= x + 150:
        if y <= pos[1] <= y + 25:
            mode = 'lagrange'
        elif y + 25 <= pos[1] <= y + 50:
            mode = 'bezier'
        elif y + 50 <= pos[1] <= y + 75:
            mode = None
            points = []
    show_menu = False


running = True
while running:
    screen.fill(WHITE)

    for pt in points:
        pygame.draw.circle(screen, RED, pt, 5)

    draw_curve()

    if show_menu:
        draw_menu(menu_pos)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Left click
                if show_menu:
                    handle_menu_click(event.pos)
                else:
                    points.append(event.pos)
            elif event.button == 3:  # Right click
                menu_pos = event.pos
                show_menu = True

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
