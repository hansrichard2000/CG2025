import pygame
import numpy as np

# Initialize pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Cubic B-Spline HW 7")
font = pygame.font.SysFont("Arial", 20)
clock = pygame.time.Clock()

# Colors
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 200, 0)
BLUE = (0, 0, 255)
GRAY = (220, 220, 220)
BLACK = (0, 0, 0)

# State variables
points = []
show_cubic = False

def draw_polyline():
    """Draw straight lines connecting the clicked points."""
    if len(points) > 1:
        pygame.draw.lines(screen, GREEN, False, points, 2)

def bspline_basis(t):
    """Return basis functions B0 to B3 evaluated at t."""
    B0 = ((1 - t) ** 3) / 6.0
    B1 = (3 * t ** 3 - 6 * t ** 2 + 4) / 6.0
    B2 = (-3 * t ** 3 + 3 * t ** 2 + 3 * t + 1) / 6.0
    B3 = t ** 3 / 6.0
    return B0, B1, B2, B3

def draw_bspline():
    """Draw Cubic B-Spline Curve using basis functions."""
    if len(points) < 4:
        return
    curve_points = []
    for i in range(len(points) - 3):
        for t in np.linspace(0, 1, 100):
            B0, B1, B2, B3 = bspline_basis(t)
            x = B0 * points[i][0] + B1 * points[i + 1][0] + B2 * points[i + 2][0] + B3 * points[i + 3][0]
            y = B0 * points[i][1] + B1 * points[i + 1][1] + B2 * points[i + 2][1] + B3 * points[i + 3][1]
            curve_points.append((x, y))
    if len(curve_points) > 1:
        pygame.draw.lines(screen, BLUE, False, curve_points, 2)

def draw_reset_button():
    """Draw the reset button on the screen."""
    pygame.draw.rect(screen, GRAY, (10, 10, 100, 30))
    pygame.draw.rect(screen, BLACK, (10, 10, 100, 30), 2)
    label = font.render("Reset", True, BLACK)
    screen.blit(label, (30, 15))


def draw_toggle_button():
    """Draw the toggle button to show Cubic B-Spline."""
    pygame.draw.rect(screen, GRAY, (120, 10, 140, 30))
    pygame.draw.rect(screen, BLACK, (120, 10, 140, 30), 2)
    label = font.render("Show B-Spline", True, BLACK)
    screen.blit(label, (130, 15))
    
running = True
while running:
    screen.fill(WHITE)

    # Draw UI
    draw_reset_button()
    draw_toggle_button()

    # Draw points and connections
    for pt in points:
        pygame.draw.circle(screen, RED, pt, 5)
    draw_polyline()
    if show_cubic:
        draw_bspline()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            mx, my = event.pos
            if event.button == 1:
                if 10 <= mx <= 110 and 10 <= my <= 40:  # Reset button
                    points = []
                    show_cubic = False
                elif 120 <= mx <= 290 and 10 <= my <= 40:  # Toggle curve button
                    show_cubic = True
                else:
                    points.append((mx, my))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()