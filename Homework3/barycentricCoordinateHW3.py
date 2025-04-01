import pygame
import numpy as np

# Initialize Pygame
pygame.init()
width, height = 600, 600
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Barycentric Coordinate HW3")
font = pygame.font.SysFont("Arial", 20)
clock = pygame.time.Clock()

# Triangle vertices
A = np.array([150, 100])
B = np.array([450, 120])
C = np.array([300, 400])


def barycentric_explicit(p, a, b, c):
    """
    Compute barycentric coordinates using explicit algebraic formula
    """
    x_p, y_p = p
    x_a, y_a = a
    x_b, y_b = b
    x_c, y_c = c

    denom = (y_b - y_c)*(x_a - x_c) + (x_c - x_b)*(y_a - y_c)
    if denom == 0:
        return None  # Degenerate triangle

    lambda1 = ((y_b - y_c)*(x_p - x_c) + (x_c - x_b)*(y_p - y_c)) / denom
    lambda2 = ((y_c - y_a)*(x_p - x_c) + (x_a - x_c)*(y_p - y_c)) / denom
    lambda3 = 1 - lambda1 - lambda2

    return lambda1, lambda2, lambda3


def draw_triangle(a, b, c):
    pygame.draw.polygon(screen, (200, 200, 200), [a, b, c], 2)


def draw_text(text, pos, color=(0, 0, 0)):
    label = font.render(text, True, color)
    screen.blit(label, pos)


# Store clicked points
clicked_points = []

running = True
while running:
    screen.fill((255, 255, 255))
    draw_triangle(A, B, C)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
            mouse_pos = np.array(pygame.mouse.get_pos())
            lambdas = barycentric_explicit(mouse_pos, A, B, C)
            if lambdas is not None and all(0 <= l <= 1 for l in lambdas):
                clicked_points.append((mouse_pos, (0, 255, 0)))  # Inside = Green
            else:
                clicked_points.append((mouse_pos, (255, 0, 0)))  # Outside = Red

    for pt, color in clicked_points:
        pygame.draw.circle(screen, color, pt, 5)

    draw_text("Click to test if point is inside triangle", (10, 10))
    pygame.display.flip()
    clock.tick(60)

pygame.quit()