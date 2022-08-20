from django.db import models

class File(models.Model):
    """File
    """
    created_by = models.ForeignKey(
        "User",
        on_delete=models.CASCADE,
        related_name="+"
    )
    file_name = models.CharField(
        max_length=50,
        unique=True
    )
