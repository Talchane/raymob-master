#include "Bouton.hpp"

Bouton::Bouton() : fontSize(30)
{

}

Bouton::Bouton(float x, float y, float width, float height, Color _color_, const char* text)
    : buttonRect{ x - width / 2, y - height / 2, width, height },
      buttonColor(_color_),
      buttonTextColor(BLACK),
      buttonText(text),
      fontSize(30)
{
}

void Bouton::Draw(bool fill) const
{
    //DrawRectangleRec(buttonRect, buttonColor);
    if (fill)
        DrawRectangleRounded(buttonRect, 0.5f, 16, buttonColor);
    else
        DrawRectangleRoundedLines(buttonRect, 0.5f, 16, 10, buttonColor);


    int textWidth = MeasureText(buttonText.c_str(), fontSize);

    // Calculer la position du texte pour le centrer
    Vector2 textPosition = {
            buttonRect.x + (buttonRect.width - textWidth) / 2,
            buttonRect.y + (buttonRect.height - fontSize) / 2
    };

    DrawText(buttonText.c_str(), textPosition.x, textPosition.y, fontSize, buttonTextColor);
}
// La marge correspond à un décalage de la zone de collision afin de rendre plus facilement atteignable le bouton
bool Bouton::isPressed(int marge) const
{
    Rectangle collisionRect{buttonRect.x - marge, buttonRect.y - marge, buttonRect.width + marge, buttonRect.height + marge};


    Vector2 touchPosition = { 0, 0 };
    bool isTouching = false;
    int touchCount = GetTouchPointCount(); // Obtenez le nombre de points de contact

    for (int i = 0; i < touchCount; i++)
    {
        touchPosition = GetTouchPosition(i);
        if (CheckCollisionPointRec(touchPosition, collisionRect))
            return true;
    }
    return false; // Le bouton n'est pas pressé
}
