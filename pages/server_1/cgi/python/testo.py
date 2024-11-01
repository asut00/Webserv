# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thelaine <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 11:37:49 by thelaine          #+#    #+#              #
#    Updated: 2024/10/01 11:40:16 by thelaine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3
from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import sys
import io
import os

content_length = os.environ.get('CONTENT_LENGTH')
print(content_length)

