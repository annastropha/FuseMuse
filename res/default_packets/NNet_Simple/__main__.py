
from __future__ import print_function
from tensorflow.python.keras.models import Sequential
from tensorflow.python.keras.layers import Dense, Activation
from tensorflow.python.keras.layers import LSTM
from tensorflow.python.keras.optimizers import RMSprop
from tensorflow.python.keras.utils import get_file
from tensorflow.python.keras.models import load_model
from tensorflow.python.keras.callbacks import EarlyStopping
import numpy as np
import random
import marshal
import math
import sys
import json
from fractions import Fraction




sys.stdin.readline() # skip mode

all_input = ""

for line in sys.stdin:
  all_input += line

  
composition = json.loads(all_input)

segs = {}

for segment in composition["pattern_segments"]:
  segs[segment["name"]] = segment
  sys.stderr.write("Added Segment " + segment["name"] + "\n")
  
output = {"name": "test part for python", "events": [], "length": 0}




START_FLAG = -100
REST_PITCH = -101
maxlen = 40
step = 10

LEARNING_RATE = 0.001
#LEARNING_RATE_DECREASE_SPEED = .1

#print('Reading Data...')
floatStrings = open('corpus_rfp_test').read().split()
text = [];
for fs in floatStrings:
  try:
    text.append(float(fs))
  except:
    fsfr = Fraction(fs)
    text.append(fsfr.numerator / fsfr.denominator)
#print('corpus length:', len(text))


#print('Processing Data...')
sets = []
rhythmvals = []
pitches = []
i = 0
while i < len(text):
  if(text[i] == START_FLAG):
    rhythmvals.append(text[i])
    pitches.append(text[i])
    i += 1
  else:
    #Rhythm to Pitch
    if(text[i] < 0):
      rhythmvals.append(text[i])
      pitches.append(REST_PITCH)
      i += 1
    else:
      rhythmvals.append(text[i])
      pitches.append(text[i+1])
      i += 2


#print('Preparing Alphabet...')
f = open('translation_data.txt','rb')
csts = marshal.load(f)
char_indices = csts['char_indices']
indices_char = csts['indices_char']
char_indices_p = csts['char_indices_p']
indices_char_p = csts['indices_char_p']
#{'char_indices': char_indices, 'indices_char': indices_char,
#              'char_indices_p': char_indices_p, 'indices_char_p': indices_char_p}, 
f.close()

def sample(preds, temperature=1.0):
    # helper function to sample an index from a probability array
    preds = np.asarray(preds).astype('float64')
    preds = np.log(preds) / temperature
    exp_preds = np.exp(preds)
    preds = exp_preds / np.sum(exp_preds)
    probas = np.random.multinomial(1, preds, 1)
    return np.argmax(probas)
    
def generate_rhythm(model_r, pitch_seed, rhythm_seed, pitch_generated, rhythm_generated, tracker):
    sentence_p = (pitch_seed + pitch_generated)[-40:] # last 40 pitches
    #Generate Rhythm
    x_pred = np.zeros((1, maxlen, len(char_indices_p.keys())))
    for t, char in enumerate(sentence_p):
        x_pred[0, t, char_indices_p[char]] = 1.
    
    preds = model_r.predict(x_pred, verbose=0)[0]
    next_index_rhythm = sample(preds, diversity)
    
    # Get next rhythm value from index
    next_char_r = indices_char[next_index_rhythm]
    
    # Add next rhythm to sentence for pitch generator
    rhythm_generated.append(next_char_r)
    tracker['total_quarter_length'] += next_char_r
    
    return next_char_r

def generate_pitch(model_p, pitch_seed, rhythm_seed, pitch_generated, rhythm_generated):
    sentence_r = (rhythm_seed + rhythm_generated)[-40:] # last 40 rhythm values
    # Generate Pitch
    x_pred = np.zeros((1, maxlen, len(char_indices)))
    for t, char in enumerate(sentence_r):
        x_pred[0, t, char_indices[char]] = 1.
    
    preds = model_p.predict(x_pred, verbose=0)[0]
    next_index_pitch = sample(preds, diversity)
    
    # Get next pitch from index
    next_char_p = indices_char_p[next_index_pitch]
    
    # Add next pitch to sentence for rhythm generator
    pitch_generated.append(next_char_p)
    
    return next_char_p

def force_triplets(rhythm_generated, tracker):
    # If we are in a triplet, enforce it
    if tracker['triplet_stage'] != 0:
      tracker['total_quarter_length'] -= rhythm_generated[-1]
      rhythm_generated[-1] = rhythm_generated[-2]; # set rhythm val to match last, which was the first or second in a triplet
      tracker['total_quarter_length'] += rhythm_generated[-1]
    # If we produced a triplet note, rotate stage
    if(math.isclose(rhythm_generated[-1], 1.0/3.0, rel_tol=.0001) or math.isclose(rhythm_generated[-1], 0.5/3.0, rel_tol=.0001)):
      tracker['triplet_stage'] += 1
      tracker['triplet_stage'] %= 3
    
def remove_eight_note_same_pitch_quadruplet(rhythm_generated, pitch_generated, tracker):
    if len(rhythm_generated) < 4:
      #if we have not generated 4 notes, return
      return
    if(rhythm_generated[-1] != 0.5 or rhythm_generated[-2] != 0.5 or rhythm_generated[-3] != 0.5 or rhythm_generated[-4] != 0.5):
      #if any of the last 4 are not eighth notes, return
      return
    if(pitch_generated[-1] != pitch_generated[-2] or pitch_generated[-1] != pitch_generated[-3] or pitch_generated[-1] != pitch_generated[-4]):
      #if the last note does not match the 3 before it, return
      return
    #we now know that the last 3 notes are the same.
    #replace the first 3 of them with a dotted quarter note
    del rhythm_generated[-3:-1]
    del pitch_generated[-3:-1]
    rhythm_generated[-2] = 1.5
    
def play_piece(pitches, rhythmVals):
    #merge the pitches and rhythms into a complete stream of returnable JSON events
    playedScore = []
    for i in range( len(pitches) ) :
        #build score to play
        quarter_length = rhythmVals[i]
        midipitch = pitches[i]
        newNote = {
            "accent": 0,
            "dotted": 0,
            "double_dotted": 0,
            "duration": 96*quarter_length,
            "fermata": 0,
            "pitch": midipitch,
            "slurred": 0,
            "staccato": 0,
            "tenuto": 0,
            "tied": 0,
            "triplet": 0,
            "type": "note"
        }
        playedScore.append(newNote)
    return playedScore


def generate_piece(diversity, model_r, model_p):
    #print('\n----- diversity:', diversity)

    generated_r = []
    seed_r = rhythmvals[start_index: start_index + maxlen]        
    generated_p = []
    seed_p = pitches[start_index: start_index + maxlen]
    sentencestr = ""
    generatedstr = ""
    for senelement in seed_p:
      sentencestr += str(senelement)
    for genelement in generated_p:
      generatedstr += str(genelement)
    #print('----- Generating with seed: "' + sentencestr + '"')
    sys.stdout.write(generatedstr)

    #for rhythm check
    #params
    measure_quarter_length = 4 # desired quarter length of measures.  Set to 4 for 4/4 time signature
    #trackers
    tracker = {'total_quarter_length': 0, 'triplet_stage': 0}

    segmentQuarterLength = 0
    
    for segmentName in composition["pattern"]:
      segment = segs[segmentName]
      segmentQuarterLength += segment["duration"] / 96.0
      while(tracker['total_quarter_length'] < segmentQuarterLength):
        next_char_r = generate_rhythm(model_r, seed_p, seed_r, generated_p, generated_r, tracker)
        
        force_triplets(generated_r, tracker)
        
        next_char_p = generate_pitch(model_p, seed_p, seed_r, generated_p, generated_r)
        
        remove_eight_note_same_pitch_quadruplet(generated_r, generated_p, tracker)
    
    playedScore = play_piece(generated_p, generated_r)
    return playedScore
    
# train the model, output generated pitches after each iteration
start_index = random.randint(0, len(rhythmvals) - maxlen - 1)
    
model_r = load_model("models-rhythmfrompitch/8.hdf5")
model_p = load_model("models-pitchfromrhythm/8.hdf5")

diversity = 0.5
generatedPiece = generate_piece(diversity, model_r, model_p)

output['events'] = generatedPiece

print(json.dumps(output))