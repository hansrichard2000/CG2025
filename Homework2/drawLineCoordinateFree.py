import pygame
from sys import exit
import numpy as np

width = 800
height = 600
pygame.init()
screen = pygame.display.set_mode((width, height), 0, 32)

pygame.display.set_caption("Drawline HW 2")

# Define the colors we will use in RGB format
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)

screen.fill(WHITE)

def drawPoint(x, y, color=BLUE, thick=5):
    """Draws a single point (pixel) at (x, y) with the given color."""
    screen.set_at((int(x), int(y)), color)
    pygame.draw.circle(screen, color, (x, y), thick)

def drawLine(x0, y0, x1, y1, color=GREEN, steps=1000):
    """
    Draws a line using a coordinate-free system with the equation:
        P(t) = (1 - t) P0 + t P1
    where t varies from 0 to 1.
    
    Parameters:
    - (x0, y0): Start point
    - (x1, y1): End point
    - color: Line color
    - steps: Number of interpolation steps (higher = smoother line)
    """
    for i in range(steps + 1):  # Generate points from t=0 to t=1
        t = i / steps  # Compute parameter t in range [0,1]
        x = (1 - t) * x0 + t * x1  # Interpolated x
        y = (1 - t) * y0 + t * y1  # Interpolated y
        drawPoint(x, y, color, thick=3)
    
    # Draw distinct colors at endpoints
    drawPoint(x0, y0, BLUE)  # First endpoint
    drawPoint(x1, y1, BLUE)  # Second endpoint

# Example usage: Draw a line from (50,100) to (400,300)
drawLine(50, 100, 400, 300)

# Refresh display
pygame.display.flip()

# Keep the window open
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

pygame.quit()