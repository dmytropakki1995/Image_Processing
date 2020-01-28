import ImageProcessingState, * as fromImage from './image';
import { combineReducers, createStore } from 'redux';

const rootReducer = combineReducers({
    ImageProcessingState
})

const store = createStore(rootReducer);

export type RootState = ReturnType<typeof rootReducer>;
export const getImageProcessingState = (state: RootState) => state.ImageProcessingState;
