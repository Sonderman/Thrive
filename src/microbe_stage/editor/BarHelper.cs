using Godot;

/// <summary>
///   Used to access the color and icon of a bar from a provided dictionary
/// </summary>
public static class BarHelper
{
    public static Color GetBarColour(string type, string name, bool production)
    {
        switch (type)
        {
            case "ATP":
            {
                foreach (var organelle in SimulationParameters.Instance.GetAllOrganelles())
                {
                    switch (name)
                    {
                        case "baseMovement":
                            return new Color(1, 0.33f, 0.14f);
                        case "osmoregulation":
                            return new Color(1, 0.84f, 0.24f);
                    }

                    if (organelle.InternalName == name)
                    {
                        if (production)
                        {
                            return new Color(organelle.ProductionColour);
                        }
                        else
                        {
                            return new Color(organelle.ConsumptionColour);
                        }
                    }
                }

                return new Color(0.68f, 0.68f, 0.68f);
            }

            default:
                return new Color(0.68f, 0.68f, 0.68f);
        }
    }

    public static Texture GetBarIcon(string type, string name)
    {
        switch (type)
        {
            case "ATP":
            {
                switch (name)
                {
                    case "baseMovement":
                        return GD.Load<Texture>("res://assets/textures/gui/bevel/baseMovementIcon.png");
                    case "osmoregulation":
                        return GD.Load<Texture>("res://assets/textures/gui/bevel/osmoIcon.png");
                }

                foreach (var organelle in SimulationParameters.Instance.GetAllOrganelles())
                {
                    if (organelle.InternalName == name)
                        return GD.Load<Texture>(organelle.IconPath);
                }

                return null;
            }

            default:
                return null;
        }
    }
}
