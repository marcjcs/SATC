import InputCounter from '../Components/InputCounter';

function ShopListItem({ title, description, image, price, addToCart }) {
  return (
    <div className="py-6">
      <div className="flex bg-white shadow-lg rounded-lg overflow-hidden">
        <div
          className="w-1/3 bg-cover"
          style={{ backgroundImage: `url(${image})` }}
        ></div>
        <div className="w-2/3 p-4">
          <h1 className="text-gray-900 font-bold text-2xl">{title}</h1>
          <p className="mt-2 text-gray-600 text-sm">{description}</p>
          <div className="flex item-center justify-between mt-3">
            <h1 className="text-gray-700 font-bold text-xl">${price}</h1>
            <button
              onClick={() => addToCart()}
              className="px-3 py-2 bg-gray-800 text-white text-xs font-bold uppercase rounded"
            >
              Add to cart
            </button>
          </div>
        </div>
      </div>
    </div>
  );
}

export default ShopListItem;
