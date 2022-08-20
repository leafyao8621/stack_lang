from django.db import models

class Example(models.Model):
    """Example
    """
    file_name = models.CharField(
        max_length=50,
        unique=True
    )
