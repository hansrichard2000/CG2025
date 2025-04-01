"""
Modified on Mar 25 2025
@author: hansrichard2000@gmail.com
@attribution: lbg@dongseo.ac.kr
"""
import pygame
import sys

# Initialize screen size and Pygame setup
width = 800
height = 600
pygame.init()
screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Draw Line HW2")

# Fonts and colors
font = pygame.font.SysFont("Arial", 18)
WHITE = (255, 255, 255)
BLUE = (0, 0, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
GRAY = (200, 200, 200)
BLACK = (0, 0, 0)

# Global state variables
screen.fill(WHITE)
clicks = []              # Stores up to 2 clicked points
draw_mode = None         # Drawing mode: None, "euclidean", "coordinate-free"
show_menu = False        # Whether to show the method popup menu
menu_pos = (0, 0)        # Position of the popup menu
points_buffer = []       # Buffer to store all clicked points regardless of mode

def drawPoint(x, y, color=BLUE, thick=5):
    """
    Draws a filled circle (point) at a given position.
    :param x: X coordinate
    :param y: Y coordinate
    :param color: RGB color tuple
    :param thick: Radius of the point
    """
    pygame.draw.circle(screen, color, (x, y), thick)

def drawLine_euclidean(x0, y0, x1, y1, color=GREEN, thick=3):
    """
    Draws a line using Euclidean interpolation.
    Uses linear interpolation between x0 and x1.
    """
    if x0 == x1:
        if y0 > y1:
            y0, y1 = y1, y0
        for y in range(y0, y1 + 1):
            pygame.draw.line(screen, color, (x0, y0), (x1, y1), thick)
    else:
        if x0 > x1:
            x0, y0, x1, y1 = x1, y1, x0, y0
        slope = (y1 - y0) / (x1 - x0)
        for x in range(x0, x1 + 1):
            y = round(y0 + slope * (x - x0))
            drawPoint(x, y, color=color, thick=thick)
    drawPoint(x0, y0, BLUE)
    drawPoint(x1, y1, BLUE)

def drawLine_coord_free(x0, y0, x1, y1, color=GREEN, thick=3):
    """
    Draws a line using coordinate-free interpolation (parametric form).
    :param x0, y0: Start point
    :param x1, y1: End point
    """
    dx = x1 - x0
    dy = y1 - y0
    steps = max(abs(dx), abs(dy))
    for i in range(steps + 1):
        t = i / steps
        x = round(x0 + dx * t)
        y = round(y0 + dy * t)
        drawPoint(x, y, color=color, thick=thick)
    drawPoint(x0, y0, BLUE)
    drawPoint(x1, y1, BLUE)

def drawMenu(pos):
    """
    Draws a popup menu with drawing mode options.
    :param pos: Tuple (x, y) of where the menu should be placed
    """
    menu_items = ["Euclidean", "Coordinate-Free", "Reset"]
    menu_width, menu_height = 160, 90
    pygame.draw.rect(screen, GRAY, (pos[0], pos[1], menu_width, menu_height))
    pygame.draw.rect(screen, BLACK, (pos[0], pos[1], menu_width, menu_height), 2)
    for i, item in enumerate(menu_items):
        text = font.render(item, True, BLACK)
        screen.blit(text, (pos[0] + 10, pos[1] + 10 + i * 25))

def handleMenuClick(pos):
    """
    Handles mouse click selection from the popup menu.
    Updates draw_mode accordingly or resets the screen.
    :param pos: Position of mouse click
    """
    global draw_mode, show_menu, points_buffer, clicks
    x, y = menu_pos
    if x <= pos[0] <= x + 160:
        if y <= pos[1] <= y + 25:
            draw_mode = "euclidean"
        elif y + 25 <= pos[1] <= y + 50:
            draw_mode = "coordinate-free"
        elif y + 50 <= pos[1] <= y + 75:
            draw_mode = None
            points_buffer = []
            clicks = []
            screen.fill(WHITE)
        show_menu = False  # Ensure menu collapses after selection
        screen.fill(WHITE)
        for pt in points_buffer:
            drawPoint(*pt, BLUE, thick=5)

        # If needed, redraw lines (depending on mode)
        if draw_mode and len(points_buffer) >= 2:
            for i in range(0, len(points_buffer) - 1, 2):
                x0, y0 = points_buffer[i]
                x1, y1 = points_buffer[i + 1]
                if draw_mode == "euclidean":
                    drawLine_euclidean(x0, y0, x1, y1)
                elif draw_mode == "coordinate-free":
                    drawLine_coord_free(x0, y0, x1, y1)

        pygame.display.flip()
    # Draw lines between consecutive points (pairwise only)
    if draw_mode and len(points_buffer) >= 2:
        for i in range(0, len(points_buffer) - 1, 2):
            x0, y0 = points_buffer[i]
            x1, y1 = points_buffer[i + 1]
            if draw_mode == "euclidean":
                drawLine_euclidean(x0, y0, x1, y1)
            elif draw_mode == "coordinate-free":
                drawLine_coord_free(x0, y0, x1, y1)

# Main event loop
pygame.display.flip()
running = True
while running:
    if show_menu:
        screen.fill(WHITE)
        for pt in points_buffer:
            drawPoint(*pt, BLUE, thick=5)
        drawMenu(menu_pos)
        pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:  # Left click
                if show_menu:
                    handleMenuClick(pygame.mouse.get_pos())
                    pygame.display.flip()
                else:
                    pos = pygame.mouse.get_pos()
                    drawPoint(*pos, BLUE, thick=5)
                    points_buffer.append(pos)
                    pygame.display.flip()

                    if draw_mode is not None:
                        clicks.append(pos)
                        if len(clicks) == 2:
                            x0, y0 = clicks[0]
                            x1, y1 = clicks[1]
                            if draw_mode == "euclidean":
                                drawLine_euclidean(x0, y0, x1, y1)
                            elif draw_mode == "coordinate-free":
                                drawLine_coord_free(x0, y0, x1, y1)
                            clicks = []
                            pygame.display.flip()

            elif event.button == 3:  # Right click shows menu
                menu_pos = pygame.mouse.get_pos()
                show_menu = True

pygame.quit()
sys.exit()
