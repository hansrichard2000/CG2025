import pygame
import numpy as np

# Initialize pygame
pygame.init()
width, height = 800, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Cubic Hermite Interpolation HW 6")
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
        
def estimate_tangents(pts):
    tangents = []
    n = len(pts)
    for i in range(n):
        if i == 0:
            dx = pts[1][0] - pts[0][0]
            dy = pts[1][1] - pts[0][1]
        elif i == n - 1:
            dx = pts[-1][0] - pts[-2][0]
            dy = pts[-1][1] - pts[-2][1]
        else:
            dx = (pts[i + 1][0] - pts[i - 1][0]) / 2
            dy = (pts[i + 1][1] - pts[i - 1][1]) / 2
        tangents.append((dx, dy))
    return tangents

def draw_hermite_curve():
    """Draw the curve generated using Bezier interpolation."""
    if len(points) < 2:
        return
    
    tangents = estimate_tangents(points)
    curve = []
    for i in range(len(points) - 1):
        p0 = np.array(points[i])
        p1 = np.array(points[i+1])
        m0 = tangents[i]
        m1 = tangents[i+1]
        for t in np.linspace(0, 1, 50):
            h00 = 2 * t**3 - 3 * t**2 + 1
            h10 = t**3 - 2 * t**2 + t
            h01 = -2 * t**3 + 3 * t**2
            h11 = t**3 - t**2
            
            x = (h00 * p0[0] + h10 * m0[0] + h01 * p1[0] + h11 * m1[0])
            y = (h00 * p0[1] + h10 * m0[1] + h01 * p1[1] + h11 * m1[1])
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
    """Draw the toggle button to show Cubic Hermite Interpolation."""
    pygame.draw.rect(screen, GRAY, (120, 10, 140, 30))
    pygame.draw.rect(screen, BLACK, (120, 10, 140, 30), 2)
    label = font.render("Show Hermite", True, BLACK)
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
        draw_hermite_curve()

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
    