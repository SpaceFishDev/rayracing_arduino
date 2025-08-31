#include <avr/io.h>
#include <util/delay.h>
#include "spi/spi.h"
#include "lcd/lcd.h"
#include "text/font.h"
#include "text/text_pixels.h"
#include "math/vector3.h"
#include "math/ray.h"
#include "raytracing/hittable.h"

double hit_sphere(vector3 center, double radius, ray r)
{
    vector3 oc = center - r.origin;
    double a = r.direction.length_squared();
    double h = dot(r.direction, oc);
    double c = oc.length_squared() - radius * radius;
    double discriminant = h * h - a * c;
    if (discriminant < 0)
    {
        return -1.0;
    }
    return (h * sqrt(discriminant)) / a;
}

vector3 ray_color(ray &r)
{
    double t = hit_sphere(vector3(0, 0, -1), 0.5, r);
    if (t > 0.0)
    {
        vector3 n = unit_vector(r.at(t) - vector3(0, 0, -1));
        return vector3(n.x + 1, n.y + 1, n.z + 1) * 0.5;
    }
    vector3 unit_direction = unit_vector(r.direction);
    double a = 0.5 * (unit_direction.y + 1.0);
    return vector3(1.0, 1.0, 1.0) * (1.0 - a) + vector3(0.5, 0.7, 1.0) * a;
}

int main(void)
{
    spi_init(&PORTD, &DDRD, PD7);
    ssd1306_init(&PORTB, &DDRB, PB0,
                 &PORTB, &DDRB, PB1);

    sort_mappings();
    double aspect_ratio = (double)(SSD1306_WIDTH / 8) / ((double)SSD1306_HEIGHT / 6);
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * aspect_ratio;
    vector3 camera_center = vector3(0, 0, 0);

    vector3 viewport_u = vector3(viewport_width, 0, 0);
    vector3 viewport_v = vector3(0, -viewport_height, 0);

    vector3 pixel_delta_u = viewport_u / (SSD1306_WIDTH / 8);
    vector3 pixel_delta_v = viewport_v / (SSD1306_HEIGHT / 6);

    vector3 viewport_upper_left = camera_center - vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    vector3 pixel00_loc = viewport_upper_left + ((pixel_delta_u + pixel_delta_v) * 0.5);

    for (int y = 0; y < SSD1306_HEIGHT / 6; y++)
    {
        for (int x = 0; x < SSD1306_WIDTH / 8; x++)
        {
            vector3 pixel_center = pixel00_loc + (pixel_delta_u * x) + (pixel_delta_v * y);
            vector3 ray_dir = pixel_center - camera_center;
            ray r = ray(camera_center, ray_dir);
            vector3 col = ray_color(r);
            col *= 255;
            int _r = col.x;
            int g = col.y;
            int b = col.z;
            put_txt_pixel(x * 8, y * 6, _r, g, b);
        }
    }
    display();
    while (1)
    {
        _delay_ms(10);
    }
    return 0;
}
