import torch
from torch import nn
from torch import jit
from torch.nn.utils.rnn import pack_padded_sequence, pad_packed_sequence

EMBEDDING_SIZE = 3
VOCAB_SIZE = 20
LSTM_SIZE = 5

class Model(nn.Module):
    def __init__(self):
        super().__init__()
        self.embedding = nn.Embedding(VOCAB_SIZE, EMBEDDING_SIZE)
        self.lstm = nn.LSTM(EMBEDDING_SIZE, LSTM_SIZE, num_layers=1, bidirectional=True)
        
    def forward(self, tokens, seq_lengths):
        embedded = self.embedding(tokens)
        
        rnn_input = pack_padded_sequence(
            embedded, seq_lengths.int(), batch_first=True,
        )
        
        rep, unused_state = self.lstm(rnn_input)
        
        unpacked, _ = pad_packed_sequence(
            rep,
            padding_value=0.0,
            batch_first=True,
            total_length=embedded.size(1),
        )
        
        return unpacked

trace_input = (
    torch.LongTensor([[1], [1]]),
    torch.LongTensor([1, 1]),
)
test_input = (
    torch.LongTensor([[1, 2, 3, 4]]),
    torch.LongTensor([4]),
)

model = Model()
model(*trace_input)
model(*test_input)

traced = jit.trace(model, trace_input)
traced(*trace_input)
traced(*test_input)  # trigger the problem, since the hidden state is constant, doesn't match the batch size.
