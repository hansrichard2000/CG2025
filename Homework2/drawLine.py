import pygame
from sys import exit
import numpy as np
    
width = 800
height = 600
pygame.init()
screen = pygame.display.set_mode((width, height), 0, 32)

pygame.display.set_caption("DrawLine HW 2")

# Define the colors we will use in RGB format
BLACK = (  0,   0,   0)
WHITE = (255, 255, 255)
BLUE =  (  0,   0, 255)
GREEN = (  0, 255,   0)
RED =   (255,   0,   0)

screen.fill(WHITE)

def drawPoint(x, y, color='BLUE', thick=5):
    """Draws a single point (pixel) at (x, y)"""
    screen.set_at((x, y), color)
    pygame.draw.circle(screen, color, (x, y), thick)
    
#HW2 implement drawLine with drawPoint
# def drawLine(x0, y0, x1, y1, color='GREEN', thick=3):
#     """
#     Draws a line from (x0, y0) to (x1, y1) using the drawPoint function.
#     Uses linear interpolation (DDA-like approach).
#     """
#     if abs(y1 - y0) > abs(x1 - x0):
#         if y0 > y1:
#             x0, y0, x1, y1 = x1, y1, x0, y0
#         for y in range(y0, y1 + 1):
#             x = round(x0 + (x1 - x0) * (y - y0) / (y1 - y0))
#             drawPoint(x, y, GREEN, thick=thick)
#     else:
#         if x0 > x1:
#             x0, y0, x1, y1 = x1, y1, x0, y0
#         for x in range(x0, x1 + 1):
#             y = round(y0 + (y1 - y0) * (x - x0) / (x1 - x0))
#             drawPoint(x, y, GREEN, thick=thick)
            
#     # Draw distinct colors at endpoints
#     drawPoint(x0, y0, BLUE)  # First endpoint in RED
#     drawPoint(x1, y1, BLUE)  # Second endpoint in GREEN

def drawLine(x0, y0, x1, y1, color='GREEN', thick=3):
    """
    Draws a line from (x0, y0) to (x1, y1) using the drawPoint function.
    Uses linear interpolation (DDA-like approach).
    """
    if x0 == x1:
        if y0 > y1:
            y0, y1 = y1, y0
        for y in range(y0, y1 + 1):
            x = round(x0 + (x1 - x0) * (y - y0) / (y1 - y0))
            drawPoint(x0, y, color=color, thick=thick)
    else:
        if x0 > x1:
            x0, y0, x1, y1 = x1, y1, x0, y0
        
        slope = (y1 - y0) / (x1 - x0)
        
        for x in range(x0, x1 + 1):
            y = round(y0 + (y1 - y0) * (x - x0) / (x1 - x0))
            drawPoint(x, round(y), GREEN, thick=thick)
            
    # Draw distinct colors at endpoints
    drawPoint(x0, y0, BLUE)  # First endpoint
    drawPoint(x1, y1, BLUE)  # Second endpoint

# Draw a line using the function
drawLine(50, 100, 400, 300)

pygame.display.flip()

# Keep the window open
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

pygame.quit()