import pygame
import numpy as np

# Initialize pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Bezier Curve HW 5")
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
show_bezier = False

def draw_polyline():
    """Draw straight lines connecting the clicked points."""
    if len(points) > 1:
        pygame.draw.lines(screen, GREEN, False, points, 2)
        
def bernstein_poly(i, n, t):
    """Bernstein polynomial."""
    return comb(n, i) * (t ** i) * ((1 - t) ** (n - i))

def comb(n, k):
    """Compute the binomial coefficient."""
    from math import comb as math_comb
    return math_comb(n, k)

def draw_bezier_curve():
    """Draw the curve generated using Bezier interpolation."""
    if len(points) < 2:
        return
    
    n = len(points) - 1
    curve = []
    for t in np.linspace(0, 1, 1000):
        x = 0.0
        y = 0.0
        for i, (px, py) in enumerate(points):
            b = bernstein_poly(i, n, t)
            x += px*b
            y += py*b
        curve.append((x, y))
    if len(curve) > 1:
        pygame.draw.lines(screen, BLUE, False, curve, 2)

def draw_reset_button():
    """Draw the reset button on the screen."""
    pygame.draw.rect(screen, GRAY, (10, 10, 100, 30))
    pygame.draw.rect(screen, BLACK, (10, 10, 100, 30), 2)
    label = font.render("Reset", True, BLACK)
    screen.blit(label, (30, 15))


def draw_toggle_button():
    """Draw the toggle button to show Bezier curve."""
    pygame.draw.rect(screen, GRAY, (120, 10, 140, 30))
    pygame.draw.rect(screen, BLACK, (120, 10, 140, 30), 2)
    label = font.render("Show Bezier", True, BLACK)
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
    if show_bezier:
        draw_bezier_curve()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            mx, my = event.pos
            if event.button == 1:
                if 10 <= mx <= 110 and 10 <= my <= 40:  # Reset button
                    points = []
                    show_bezier = False
                elif 120 <= mx <= 290 and 10 <= my <= 40:  # Toggle curve button
                    show_bezier = True
                else:
                    points.append((mx, my))

    pygame.display.flip()
    clock.tick(60)

pygame.quit()