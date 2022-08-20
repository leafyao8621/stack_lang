from rest_framework import serializers
from ..models import User

class UserSerializer(serializers.ModelSerializer):
    """Basic Serializer for User
    """
    class Meta:
        model = User
        fields = (
            "id",
            "user_name"
        )
