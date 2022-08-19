
from django.db import models

class User(models.Model):
    """User
    """
    user_name = models.CharField(
        max_length=50,
        unique=True
    )
