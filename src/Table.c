#include "Table.h"
#include <stdlib.h>
#include <assert.h>

void Table_Init(Table* table, int screen_width, int screen_height)
{
    assert(NULL != table);

    // Draw the main felt in the middle.
    Rectangle felt = { .width = screen_width * 0.75, .height = screen_height * 0.75 };
    felt.x = (screen_width - felt.width) / 2;
    felt.y = (screen_height - felt.height) / 2;

    // Draw upper rail.
    Rectangle rail_upper = { .width = felt.width, .height = 0.075 * felt.height };
    rail_upper.x = felt.x;
    rail_upper.y = felt.y - rail_upper.height;

    // Draw left rail.
    Rectangle rail_left = { .width = 0.075 * felt.height, .height = felt.height };
    rail_left.x = felt.x - rail_left.width;
    rail_left.y = felt.y;

    // Draw lower rail.
    Rectangle rail_lower = rail_upper;
    rail_lower.x = felt.x;
    rail_lower.y = felt.y + felt.height; 

    // Draw right rail.
    Rectangle rail_right = rail_left;
    rail_right.x = felt.x + felt.width;
    rail_right.y = felt.y;

    // Draw top-left connector.
    Rectangle top_left_connector = { 
        .width = rail_left.width, 
        .height = rail_upper.height,
        .x = rail_left.x,
        .y = rail_upper.y
    };

    // Draw top-right connector.
    Rectangle top_right_connector = { 
        .width = rail_right.width, 
        .height = rail_upper.height,
        .x = rail_right.x,
        .y = rail_upper.y
    };

    // Draw bottom-left connector.
    Rectangle bottom_left_connector = { 
        .width = rail_left.width, 
        .height = rail_lower.height,
        .x = rail_left.x,
        .y = rail_lower.y
    };

    // Draw bottom-right connector.
    Rectangle bottom_right_connector = { 
        .width = rail_right.width, 
        .height = rail_lower.height,
        .x = rail_right.x,
        .y = rail_lower.y
    };

    table->felt = felt;
    table->rail_upper = rail_upper;
    table->rail_lower = rail_lower;
    table->rail_left= rail_left;
    table->rail_right = rail_right;
    table->top_left_connector = top_left_connector;
    table->top_right_connector = top_right_connector;
    table->bottom_left_connector = bottom_left_connector;
    table->bottom_right_connector = bottom_right_connector;
    
    // Create pockets.
    float const pocket_scale = 0.65f;
    int const pocket_offset = 15;

    table->pockets[0].x = top_left_connector.x + (top_left_connector.width * pocket_scale) + pocket_offset;
    table->pockets[0].y = top_left_connector.y + (top_left_connector.height * pocket_scale) + pocket_offset;
    table->pockets[0].radius = top_left_connector.width * pocket_scale; 

    table->pockets[1].x = top_right_connector.x + (top_right_connector.width * pocket_scale) - pocket_offset;     
    table->pockets[1].y = top_right_connector.y + (top_right_connector.height * pocket_scale) + pocket_offset;     
    table->pockets[1].radius = top_right_connector.width * pocket_scale;     

    table->pockets[2].x = bottom_left_connector.x + (bottom_left_connector.width * pocket_scale) + pocket_offset;  
    table->pockets[2].y = bottom_left_connector.y + (bottom_left_connector.height * pocket_scale) - pocket_offset;  
    table->pockets[2].radius = top_left_connector.width * pocket_scale;  

    table->pockets[3].x = bottom_right_connector.x + (bottom_right_connector.width * pocket_scale) - pocket_offset; 
    table->pockets[3].y = bottom_right_connector.y + (bottom_right_connector.height * pocket_scale) - pocket_offset; 
    table->pockets[3].radius = top_left_connector.width * pocket_scale; 

    table->pockets[4].x = screen_width / 2; 
    table->pockets[4].y = bottom_right_connector.y + (bottom_right_connector.height * pocket_scale) - pocket_offset; 
    table->pockets[4].radius = top_left_connector.width * pocket_scale; 

    table->pockets[5].x = screen_width / 2; 
    table->pockets[5].y = top_right_connector.y + (top_right_connector.height * pocket_scale) + pocket_offset; 
    table->pockets[5].radius = top_left_connector.width * pocket_scale; 

    // Set default friction coefficient.
    table->friction_coeff = 0.01f;
}

void Table_Draw(Table* table)
{
    assert(NULL != table);

    DrawRectangleRec(table->felt, GREEN);
    DrawRectangleRec(table->rail_left, BROWN); 
    DrawRectangleRec(table->rail_lower, BROWN); 
    DrawRectangleRec(table->rail_right, BROWN); 
    DrawRectangleRec(table->rail_upper, BROWN); 
    DrawRectangleRec(table->top_left_connector, BROWN); 
    DrawRectangleRec(table->top_right_connector, BROWN);
    DrawRectangleRec(table->bottom_left_connector, BROWN);
    DrawRectangleRec(table->bottom_right_connector, BROWN);
 
    for (int i = 0; i < 6; i++)
    {
        Pocket* pocket = &table->pockets[i];
        DrawCircle(pocket->x, pocket->y, pocket->radius, BLACK);
    }
}