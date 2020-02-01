import ImageProcessingState, * as fromImage from './image';
import { combineReducers, createStore, compose} from 'redux';

declare global {
    interface Window {
      __REDUX_DEVTOOLS_EXTENSION_COMPOSE__?: typeof compose;
    }
  }
  
const composeEnhancers = window.__REDUX_DEVTOOLS_EXTENSION_COMPOSE__ || compose;
const rootReducer = combineReducers({
    ImageProcessingState
})

export const store = createStore(rootReducer, composeEnhancers());

export type RootState = ReturnType<typeof rootReducer>;
export const getImageProcessingState = (state: RootState) => state.ImageProcessingState;
