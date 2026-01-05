import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Embedding, Lambda
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.preprocessing.sequence import pad_sequences
import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import PCA

import string
import time


EMBEDDING_SIZE = 256
WINDOW_SIZE = 2
BATCH = 50000

# Define the corpus

# corpus = ['In a small village surrounded by mountains, people lived in harmony with nature. The seasons changed gently, bringing new colors and sounds to the valley. In spring, the flowers bloomed, and birds sang joyfully, filling the air with music. Farmers planted crops in neat rows, while children played near the streams, their laughter echoing through the hills. Summer brought warmth, and the fields turned green with life. The village buzzed with activity, as people harvested fruits and vegetables. Autumn arrived with a cool breeze, turning the leaves into shades of gold and red. The village prepared for winter, gathering wood and storing food. Snow covered the land, and the world seemed to slow down. Families gathered around fires, sharing stories of the past and dreaming of the future. Despite the challenges of each season, the villagers remained hopeful and resilient, always finding joy in the simple moments of life.']

def custom_standardization(input_data):
    input_data.lower()
    # Create translation table
    translator = str.maketrans('', '', string.punctuation)

    # Remove punctuation
    return input_data.lower().translate(translator)

f = open("war_and_peace.txt", "r")
input_data = f.read()
data = custom_standardization(input_data)
corpus = [data]

# print(corpus)
 
# Convert the corpus to a sequence of integers
tokenizer = Tokenizer()
tokenizer.fit_on_texts(corpus)
sequences = tokenizer.texts_to_sequences(corpus)
print("After converting our words in the corpus into vector of integers:")
print(len(sequences[0]))

# # ----

word_index = tokenizer.word_index
index_word = {v: k for k, v in word_index.items()}


# Define the parameters
vocab_size = len(tokenizer.word_index) + 1
print("vocab size: ", vocab_size)

# Generate the context-target pairs
contexts = []
targets = []
for sequence in sequences:
    for i in range(WINDOW_SIZE, len(sequence[:BATCH]) - WINDOW_SIZE):
        context = sequence[i - WINDOW_SIZE:i] +\
            sequence[i + 1:i + WINDOW_SIZE + 1]
        target = sequence[i]
        contexts.append(context)
        targets.append(target)

print(" T ----- ", len(targets))

# Convert the contexts and targets to numpy arrays
X = np.array(contexts, dtype=np.float16)[:BATCH]

y = np.zeros((len(X), vocab_size), dtype=np.float16)[:BATCH]
for i in range(0, len(targets)):
    y[i][targets[i]] = 1


print(" X ----- ", len(X))
print(" Y ----- ", len(y))

# np.save("x.npy", X)
# np.save("y.npy", y)


# Define the CBOW model
model = Sequential()
model.add(Embedding(input_dim=vocab_size,
                    output_dim=EMBEDDING_SIZE,
                    input_length=2*WINDOW_SIZE))
model.add(Lambda(lambda x: tf.reduce_mean(x, axis=1)))
model.add(Dense(units=vocab_size, activation='softmax'))

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.summary()


# Fit model
print(" ----- Model training\n")
# tf.debugging.set_log_device_placement(True)
model.fit(X, y, epochs=10, verbose=1)

model.save_weights('cbow_weights.h5')