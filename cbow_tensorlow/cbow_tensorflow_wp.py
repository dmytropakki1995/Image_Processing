import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Embedding, Lambda
from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences
import numpy as np

import string

#tf.debugging.set_log_device_placement(True)

# text = """
# In a small village surrounded by mountains, people lived in harmony with nature. The seasons changed gently, bringing new colors and sounds to the valley. In spring, the flowers bloomed, and birds sang joyfully, 
# filling the air with music. Farmers planted crops in neat rows, while children played near the streams, their laughter echoing through the hills. 
# Summer brought warmth, and the fields turned green with life. The village buzzed with activity, as people harvested fruits and vegetables. 
# Autumn arrived with a cool breeze, turning the leaves into shades of gold and red. The village prepared for winter, gathering wood and storing food. 
# Snow covered the land, and the world seemed to slow down. Families gathered around fires, sharing stories of the past and dreaming of the future. 
# Despite the challenges of each season, the villagers remained hopeful and resilient, always finding joy in the simple moments of life.
# """

# # Preprocessing: Tokenizing the text
# tokenizer = Tokenizer()
# tokenizer.fit_on_texts([text])
# word2idx = tokenizer.word_index
# vocab_size = len(word2idx) + 1

# sequences = []
# for sentence in text.split('.'):
#     sentence_tokens = tokenizer.texts_to_sequences([sentence])[0]
#     if len(sentence_tokens) > 4:
#         for i in range(2, len(sentence_tokens) - 2):
#             context = [sentence_tokens[i - 2], sentence_tokens[i - 1], sentence_tokens[i + 1], sentence_tokens[i + 2]]
#             target = sentence_tokens[i]
#             sequences.append((context, target))
            
# # print(sequences)

# def generate_data(sequences, vocab_size):
#     X = []
#     y = []
#     for context, target in sequences:
#         X.append(context)
#         y.append(target)
#     X = np.array(X)
#     y = to_categorical(y, num_classes=vocab_size)
#     return X, y

# X, y = generate_data(sequences, vocab_size)


# In[3]:


# f = open("/Users/dpakki/tempNotes/pythonNotebook/shakespeare.txt", "r")
# input_data = f.read()
# data = custom_standardization(input_data)
# corpus = [data]
# # print(corpus)


# In[9]:


# Define the corpus
# corpus = ['In a small village surrounded by mountains, people lived in harmony with nature. The seasons changed gently, bringing new colors and sounds to the valley. In spring, the flowers bloomed, and birds sang joyfully, filling the air with music. Farmers planted crops in neat rows, while children played near the streams, their laughter echoing through the hills. Summer brought warmth, and the fields turned green with life. The village buzzed with activity, as people harvested fruits and vegetables. Autumn arrived with a cool breeze, turning the leaves into shades of gold and red. The village prepared for winter, gathering wood and storing food. Snow covered the land, and the world seemed to slow down. Families gathered around fires, sharing stories of the past and dreaming of the future. Despite the challenges of each season, the villagers remained hopeful and resilient, always finding joy in the simple moments of life.']

def custom_standardization(input_data):
    input_data.lower()
    # Create translation table
    translator = str.maketrans('', '', string.punctuation)

    # Remove punctuation
    return input_data.lower().translate(translator)

f = open("comb.txt", "r")
input_data = f.read()
data = custom_standardization(input_data)
corpus = [data]
 
# Convert the corpus to a sequence of integers
tokenizer = Tokenizer()
tokenizer.fit_on_texts(corpus)
sequences = tokenizer.texts_to_sequences(corpus)
# print("After converting our words in the corpus \
# into vector of integers:")
# print(sequences)


# In[10]:


word_index = tokenizer.word_index
index_word = {v: k for k, v in word_index.items()}


# In[11]:


# len(sequences[0])
len(sequences[0])


# In[12]:


# Define the parameters
vocab_size = len(tokenizer.word_index) + 1
embedding_size = 256
window_size = 3

print("vocab size: ", vocab_size)

# Generate the context-target pairs
contexts = []
targets = []
for sequence in sequences:
    for i in range(window_size, len(sequence) - window_size):
        context = sequence[i - window_size:i] +\
            sequence[i + 1:i + window_size + 1]
        target = sequence[i]
        contexts.append(context)
        targets.append(target)

# Convert the contexts and targets to numpy arrays
X = np.array(contexts)

# y = np.zeros((len(X), vocab_size))
# for i in range(0, len(targets)):
#     y[i][targets[i]] = 1

y = np.array(targets)

# print(X[1], y[0])

# Define the CBOW model
model = Sequential()
model.add(Embedding(input_dim=vocab_size,
                    output_dim=embedding_size,
                    input_length=2*window_size))
model.add(Lambda(lambda x: tf.reduce_mean(x, axis=1)))
model.add(Dense(units=vocab_size, activation='softmax'))

# Compile the model
#model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

# In[13]:


len(contexts)



model.summary()


model.fit(X, y, epochs=200, verbose=2)

model.save_weights('cbow_weights.h5')

# Load the pre-trained weights
#model.load_weights('cbow_weights.h5')
#-------------------------------------------------
# W1 = model.get_weights()[0]
# W2 = model.get_weights()[1]

# len(W1)

# def relu(x):
#     return max(0.0, x)

# weights_new = model.get_weights()[0].copy()
# threshold = 0.8

# for i in range(len(weights_new)):
#     for j in range(len(weights_new[i])):
#         if relu(weights_new[i][j]) > threshold:
#             weights_new[i][j] = 1
#         else:
#             weights_new[i][j] = 0


# index = 35
# count = 0
# for i in weights_new[index]:
#     if i == 1.0:
#         count += 1

# print(count)


# word = "if"
# index = word_index[word]
# print(f"{word} ---> {weights_new[index]}")


# # After training, you can test the model's predictions for a given context
# def predict_target(context_words):
#     context_sequence = tokenizer.texts_to_sequences([context_words])[0]
#     context_sequence = pad_sequences([context_sequence], maxlen=8)
#     prediction = model.predict(context_sequence)
#     predicted_word_idx = np.argmax(prediction)
#     predicted_word = tokenizer.index_word[predicted_word_idx]
#     return predicted_word

# # Example prediction
# context_words = ['if', 'it', 'is', 'raining', 'I', 'take', 'an', 'umbrella'] # 'outside'
# predicted_word = predict_target(context_words)
# print(f"Predicted word for context {context_words}: {predicted_word}")




