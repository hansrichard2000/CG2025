import pygame
import numpy as np

# Initialize pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Lagrange Interpolation HW 4")
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
show_lagrange = False

def lagrange_interpolate(x, t, cp):
    """
    Perform Lagrange interpolation at a specific x position.

    Args:
        x (float): Target x position.
        t (list of float): x-coordinates of control points.
        cp (list of tuple): control points (x, y).

    Returns:
        float: Interpolated y value.
    """
    result = 0.0
    n = len(t)
    for i in range(n):
        numerator = 1.0
        denominator = 1.0
        for j in range(n):
            if j != i:
                numerator *= (x - t[j])
                denominator *= (t[i] - t[j])
        result += cp[i][1] * (numerator / denominator)
    return result


def draw_polyline():
    """Draw straight lines connecting the clicked points."""
    if len(points) > 1:
        pygame.draw.lines(screen, GREEN, False, points, 2)


def draw_lagrange_curve():
    """Draw the curve generated using Lagrange interpolation."""
    if len(points) < 2:
        return

    # Use uniform parameterization based on input order
    t = list(range(len(points)))
    cp = points
    curve = []
    for x in np.linspace(0, len(points) - 1, 1000):
        x_val = 0.0
        y_val = 0.0
        for i in range(len(cp)):
            numerator = 1.0
            denominator = 1.0
            for j in range(len(cp)):
                if j != i:
                    numerator *= (x - t[j])
                    denominator *= (t[i] - t[j])
            basis = numerator / denominator
            x_val += cp[i][0] * basis
            y_val += cp[i][1] * basis
        curve.append((x_val, y_val))

    if len(curve) > 1:
        pygame.draw.lines(screen, BLUE, False, curve, 2)


def draw_reset_button():
    """Draw the reset button on the screen."""
    pygame.draw.rect(screen, GRAY, (10, 10, 100, 30))
    pygame.draw.rect(screen, BLACK, (10, 10, 100, 30), 2)
    label = font.render("Reset", True, BLACK)
    screen.blit(label, (30, 15))


def draw_toggle_button():
    """Draw the toggle button to show Lagrange curve."""
    pygame.draw.rect(screen, GRAY, (120, 10, 170, 30))
    pygame.draw.rect(screen, BLACK, (120, 10, 170, 30), 2)
    label = font.render("Show Lagrange", True, BLACK)
    screen.blit(label, (130, 15))


running = True
while running:
    screen.fill(WHITE)

    # Draw UI
    draw_reset_button()
    draw_toggle_button()

    # Draw control points
    for pt in points:
        pygame.draw.circle(screen, RED, pt, 5)

    draw_polyline()

    if show_lagrange:
        draw_lagrange_curve()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            mx, my = event.pos

            if 10 <= mx <= 110 and 10 <= my <= 40:  # Reset button
                points = []
                show_lagrange = False

            elif 120 <= mx <= 290 and 10 <= my <= 40:  # Toggle curve button
                show_lagrange = True

            else:  # Add point
                points.append((mx, my))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()
